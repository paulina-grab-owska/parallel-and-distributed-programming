#include <stdio.h>
#include <omp.h>

#define ARRAY_SIZE 12

int main() {
    int array[ARRAY_SIZE];

    // tablica
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = i;
    }

    printf("Static schedule:\n");
    #pragma omp parallel for schedule(static) default(none) shared(array)
    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("Thread %d processes index %d\n", omp_get_thread_num(), i);
    }

    printf("\nStatic(3) schedule:\n");
    #pragma omp parallel for schedule(static, 3) default(none) shared(array)
    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("Thread %d processes index %d\n", omp_get_thread_num(), i);
    }

    printf("\nDynamic schedule:\n");
    #pragma omp parallel for schedule(dynamic) default(none) shared(array)
    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("Thread %d processes index %d\n", omp_get_thread_num(), i);
    }

    printf("\nDynamic(2) schedule:\n");
    #pragma omp parallel for schedule(dynamic, 2) default(none) shared(array)
    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("Thread %d processes index %d\n", omp_get_thread_num(), i);
    }

    return 0;
}
