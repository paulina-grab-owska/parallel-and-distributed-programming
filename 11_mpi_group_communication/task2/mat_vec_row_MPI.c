#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "mpi.h"

#define WYMIAR 10080
#define ROZMIAR (WYMIAR*WYMIAR)

void mat_vec(double* a, double* x, double* y, int n, int nt);

int
main ( int argc, char** argv )
{
  static double x[WYMIAR],y[WYMIAR],z[WYMIAR];
  double *a;
  double t1;
  int n,nt,i,j;
  const int ione=1;
  const double done=1.0;
  const double dzero=0.0;
  double *a_local; // - > dodane

  int rank, size, source, dest, tag=0;
  int n_wier, n_wier_last;
  MPI_Status status;

  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  MPI_Comm_size( MPI_COMM_WORLD, &size );

  //////////////////////////////////////////////////////////////////////////////////// SEKWENCYJNIE
  n=WYMIAR;
  for(i=0;i<WYMIAR;i++) x[i]=0.0;

  if(rank==0){

    a = (double *) malloc((ROZMIAR+1)*sizeof(double));

    for(i=0;i<ROZMIAR;i++) a[i]=1.0*i;
    for(i=0;i<WYMIAR;i++) x[i]=1.0*(WYMIAR-i);

    nt=1;

    printf("\nPoczatek (wykonanie sekwencyjne)\n\n");

    t1 = MPI_Wtime();
    mat_vec(a,x,y,n,nt);
    t1 = MPI_Wtime() - t1;

    printf("\tczas wykonania (zaburzony przez MPI?): %lf, (Gflop/s: %lf), (GB/s: %lf)\n\n",
       t1, 2.0e-9*ROZMIAR/t1, (1.0+1.0/n)*8.0e-9*ROZMIAR/t1);
  }
  //////////////////////////////////////////////////////////////////////////////////// Rozproszona dekompozycja blokowo-wierszowa
  if(size>1) {
    for(i=0;i<WYMIAR;i++) z[i]=0.0;

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD );

    n_wier = ceil(WYMIAR / size);
    n_wier_last = WYMIAR - n_wier*(size-1);

    if(n_wier!=n_wier_last){
      printf("This version does not work with WYMIAR not a multiple of size!\n");
      MPI_Finalize();
      exit(0);
    }

    a_local = (double *) malloc(WYMIAR*n_wier*sizeof(double));
    for(i=0;i<WYMIAR*n_wier;i++) a_local[i]=0.0;

    if(rank==0){
     for(i=0;i<WYMIAR*n_wier;i++) a_local[i]=a[i];
    }
    //----------------------------------------------------------------------- Usunięty kod
    // if(rank==0){

    //  for(i=0;i<WYMIAR*n_wier;i++) a_local[i]=a[i];

    //   for(i=1;i<size-1;i++) {
      //     MPI_Send( &a[i*WYMIAR*n_wier], n_wier*WYMIAR, MPI_DOUBLE, i, tag, MPI_COMM_WORLD );
      //     MPI_Send( &x[i*n_wier], n_wier, MPI_DOUBLE, i, tag, MPI_COMM_WORLD );
    //   }


    //   MPI_Send( &a[(size-1)*WYMIAR*n_wier], n_wier_last*WYMIAR, MPI_DOUBLE, size-1, tag, MPI_COMM_WORLD );
    //   MPI_Send( &x[(size-1)*n_wier], n_wier_last, MPI_DOUBLE, size-1, tag, MPI_COMM_WORLD );

    // }
    // else {
    //   for(i=0;i<WYMIAR;i++) x[i]=0.0;
    //   source = 0;
    //   if(rank<size-1) {

      //     MPI_Recv( a_local, n_wier*WYMIAR, MPI_DOUBLE, source,
        //     MPI_ANY_TAG, MPI_COMM_WORLD, &status );
      //     MPI_Recv( &x[rank*n_wier], n_wier, MPI_DOUBLE, source,
        //     MPI_ANY_TAG, MPI_COMM_WORLD, &status );
    //   }
    //   else {
      //     MPI_Recv( a_local, n_wier_last*WYMIAR, MPI_DOUBLE, source,
        //     MPI_ANY_TAG, MPI_COMM_WORLD, &status );
      //     MPI_Recv( &x[(size-1)*n_wier], n_wier_last, MPI_DOUBLE, source,
        //     MPI_ANY_TAG, MPI_COMM_WORLD, &status );
    //   }
    // }
    //-----------------------------------------------------------------------

    MPI_Scatter(a, n_wier*WYMIAR, MPI_DOUBLE, a_local, n_wier*WYMIAR, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(x, n_wier, MPI_DOUBLE, &x[rank*n_wier], n_wier, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if(rank==0) {
      printf("Starting MPI matrix-vector product with block row decomposition!\n\n");
      t1 = MPI_Wtime();
    }

    MPI_Allgather( MPI_IN_PLACE, n_wier, MPI_DOUBLE, x, n_wier, MPI_DOUBLE, MPI_COMM_WORLD );

    for(i=0;i<n_wier;i++){
      double t=0.0;
      int ni = n*i;
      for(j=0;j<n;j++) {
          t += a_local[ni+j] * x[j];
      }
      z[i] = t;
    }
    MPI_Barrier(MPI_COMM_WORLD);

    if(rank==0) {
      t1 = MPI_Wtime() - t1;
      printf("Wersja rownolegla MPI z dekompozycją wierszową blokową\n");
      printf("\tczas wykonania: %lf, (Gflop/s: %lf), (GB/s: %lf)\n\n",
         t1, 2.0e-9*ROZMIAR/t1, (1.0+1.0/n)*8.0e-9*ROZMIAR/t1);
    }

    MPI_Gather(z, n_wier, MPI_DOUBLE, z, n_wier, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  }

    // Sprawdzenie poprawnosci wynikow:
    // if(rank>0) {
    //   MPI_Send( z, n_wier, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD );
    // }
    // else {
    //   for(i=1;i<size;i++) {
      //     MPI_Recv( &z[i*n_wier], n_wier, MPI_DOUBLE, i, tag, MPI_COMM_WORLD, &status);
    //   }
    // }
    //-----------------------------------------------------------------------
    if(rank==0){
      for(i=0;i<WYMIAR;i++){
          if(fabs(y[i]-z[i])>1.e-9*z[i]) {
          printf("---> Blad! i=%d, y[i]=%lf, z[i]=%lf\n",i, y[i], z[i]);
          }
      }
    }
  MPI_Finalize();
  return(0);
}

////////////////////////////////////////////////////////////////////////// ALGORYTMY
void mat_vec(double* a, double* x, double* y, int nn, int nt)
{


  register int n=nn;
  register int i;
  for(i=0;i<n;i+=2){
    register double ty1 = 0;
    register double ty2 = 0;
    register int j;
    for(j=0;j<n;j+=2){
      register double t0=x[j];
      register double t1=x[j+1];
      register int k= i*n+j;
      ty1  +=a[k]*t0    +a[k+1]*t1 ;
      ty2  +=a[k+n]*t0  +a[k+1+n]*t1;
    }
    y[i]  = ty1;
    y[i+1]+=ty2;
  }
}
