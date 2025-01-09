#include <stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include <string.h>
#include "mpi.h"

int main( int argc, char** argv ){

  int rank, ranksent, size, source, dest, tag, i;

  char hostname[256], received_hostname[256];

  MPI_Status status;

  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  MPI_Comm_size( MPI_COMM_WORLD, &size );

  if(size>1){

        gethostname(hostname, sizeof(hostname));

    if( rank != 0 ){ dest=0; tag=0;

       MPI_Send( &rank, 1, MPI_INT, dest, tag, MPI_COMM_WORLD );
        MPI_Send(hostname, 256, MPI_CHAR, dest, tag, MPI_COMM_WORLD);

    } else {

      for( i=1; i<size; i++ ) {

        MPI_Recv( &ranksent, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status );
        printf("Dane od procesu o randze (status.MPI_SOURCE ->) %d: %d (i=%d)\n", status.MPI_SOURCE, ranksent, i );

        MPI_Recv(received_hostname, 256, MPI_CHAR, status.MPI_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      printf("Dane od procesu %d: hostname = %s\n", status.MPI_SOURCE, received_hostname);
}

    }

  }
  else{
        printf("Pojedynczy proces o randze: %d (brak komunikatów)\n", rank);
  }


  MPI_Finalize();
  return(0);

}
