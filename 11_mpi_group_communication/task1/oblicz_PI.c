#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "mpi.h"

#define SCALAR double

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

int main(int argc, char** argv){
  int max_liczba_wyrazow = 0;
  int root = 0;
  int start, end, move;
  int rank, size;
  MPI_Status status;

  MPI_Init (&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if(rank==0){
        printf("\nPodaj maksymalną liczbę wyrazów do obliczenia przybliżenia PI:\n");
        scanf("%d", &max_liczba_wyrazow);
  }

  // proces o randze root = 0
  MPI_Bcast(&max_liczba_wyrazow, 1, MPI_INT, root, MPI_COMM_WORLD);

  move = ceil(max_liczba_wyrazow/size);
  start = rank * move;
  end = start + move - 1;

  SCALAR suma_plus = 0.0;
  SCALAR suma_minus = 0.0;
  double suma_procesu = 0.0;

  printf("Proces o ID = %d, start: %d, koniec %d:\n", rank, start, end);

  int i=0;
  for(i = start; i <= end; i++){
    int j = 1 + 4*i;
    suma_plus += 1.0/j;
    suma_minus += 1.0/(j+2.0);
  }
    suma_procesu = 4*(suma_plus - suma_minus);


  double suma = 0.0;
  MPI_Reduce(&suma_procesu, &suma, 1, MPI_DOUBLE, MPI_SUM, root, MPI_COMM_WORLD);
//MPI_Allreduce(&suma_procesu, &suma, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

//  if(rank==0){
        printf("PI obliczone:\t\t\t%20.15lf\n", suma);
          printf("PI z biblioteki matematycznej:\t%20.15lf\n", M_PI);
if(rank==0){
          printf("Roznica:\t\t\t%20.15lf\n", M_PI-suma);
  }

  MPI_Finalize();
  return 0;
}

