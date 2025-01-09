#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mpi.h"

int main(int argc, char** argv) {
    int rank, size, prev, next, tag = 0;
    int value = 0;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    prev = (rank - 1 + size) % size;
    next = (rank + 1) % size;

    int ring = 0; // open
//    int ring = 1; //closed

    if (size < 2) {
        MPI_Finalize();
        return 0;
    }

    if (rank == 0) {
        value = 100;
        printf("Proces %d rozpoczyna sztafetę z wartością: %d\n", rank, value);

        MPI_Send(&value, 1, MPI_INT, next, tag, MPI_COMM_WORLD);
        printf("Proces %d wysłał liczbę %d do procesu %d\n", rank, value, next);

        if (ring) {
            MPI_Recv(&value, 1, MPI_INT, prev, tag, MPI_COMM_WORLD, &status);
            printf("Proces %d odebrał liczbę %d od procesu %d\n", rank, value, prev);
            printf("Sztafeta zakończona w procesie %d.\n", rank);
        }
    } else {

        MPI_Recv(&value, 1, MPI_INT, prev, tag, MPI_COMM_WORLD, &status);
        printf("Proces %d odebrał liczbę %d od procesu %d\n", rank, value, prev);

        value += 1;

        if (rank == size - 1 && !ring) {
          printf("Proces %d zakończył sztafetę z wartością: %d\n", rank, value);
        } else {
            MPI_Send(&value, 1, MPI_INT, next, tag, MPI_COMM_WORLD);
            printf("Proces %d wysłał liczbę %d do procesu %d\n", rank, value, next);
        }
    }

    MPI_Finalize();
    return 0;
}
