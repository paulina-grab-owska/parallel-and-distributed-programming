#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"

typedef struct {
    int id;
    float val;
    char word[10];
} CustomStruct;

int main(int argc, char** argv) {
    int rank, size, next, prev, position;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    prev = (rank - 1 + size) % size;
    next = (rank + 1) % size;

    int buffer_size = sizeof(CustomStruct) + MPI_BSEND_OVERHEAD;
    char* buffer = (char*)malloc(buffer_size);

    CustomStruct data;
    position = 0;

    int token = 1;

    if (rank == 0) {
        data.id = 1;
        data.val = 21.12;
        strcpy(data.word, "money");

        printf("Proccess %d: sending structure -> id: %d, val: %.2f, word: %s\n",
               rank, data.id, data.val, data.word);

        position = 0;
        MPI_Pack(&token, 1, MPI_INT, buffer, buffer_size, &position, MPI_COMM_WORLD);
        MPI_Pack(&data.id, 1, MPI_INT, buffer, buffer_size, &position, MPI_COMM_WORLD);
        MPI_Pack(&data.val, 1, MPI_FLOAT, buffer, buffer_size, &position, MPI_COMM_WORLD);
        MPI_Pack(data.word, strlen(data.word) + 1, MPI_CHAR, buffer, buffer_size, &position, MPI_COMM_WORLD);

        MPI_Send(buffer, position, MPI_PACKED, next, 0, MPI_COMM_WORLD);
    }

        position = 0;


        MPI_Recv(buffer, buffer_size, MPI_PACKED, prev, 0, MPI_COMM_WORLD, &status);


        MPI_Unpack(buffer, buffer_size, &position, &token, 1, MPI_INT, MPI_COMM_WORLD);
        MPI_Unpack(buffer, buffer_size, &position, &data.id, 1, MPI_INT, MPI_COMM_WORLD);
        MPI_Unpack(buffer, buffer_size, &position, &data.val, 1, MPI_FLOAT, MPI_COMM_WORLD);
        MPI_Unpack(buffer, buffer_size, &position, data.word, 10, MPI_CHAR, MPI_COMM_WORLD);


        printf("Proccess %d: received strcture -> id: %d, val: %.2f, word: %s\n",
               rank, data.id, data.val, data.word);

        if(!(rank == 0 && prev == size -1)){
        data.id += 1;
        data.val += 1.0;
        strcat(data.word, "$");

        printf("Procceses %d:  modificating structure -> id: %d, val: %.2f, word: %s\n",
               rank, data.id, data.val, data.word);
        }

        if (rank == 0) {
            token = 0;
        }

        position = 0;
        MPI_Pack(&token, 1, MPI_INT, buffer, buffer_size, &position, MPI_COMM_WORLD);
        MPI_Pack(&data.id, 1, MPI_INT, buffer, buffer_size, &position, MPI_COMM_WORLD);
        MPI_Pack(&data.val, 1, MPI_FLOAT, buffer, buffer_size, &position, MPI_COMM_WORLD);
        MPI_Pack(data.word, strlen(data.word) + 1, MPI_CHAR, buffer, buffer_size, &position, MPI_COMM_WORLD);

        MPI_Send(buffer, position, MPI_PACKED, next, 0, MPI_COMM_WORLD);

    free(buffer);
    MPI_Finalize();
    return 0;
}
