#include <stdio.h>
#include <omp.h>

#define ROWS 9
#define COLS 10

int main() {
    int array[ROWS][COLS];
    int total_sum = 0;

    // tablica
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            array[i][j] = i + j; // przykladowe
        }
    }

    printf("Tablica 2D:\n");
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%3d ", array[i][j]);
        }
        printf("\n");
    }

    // Dekompocycja wierszowa
    printf("\nSumowanie (dekompozycja wierszowa):\n");
    #pragma omp parallel for reduction(+:total_sum) default(none) shared(array)
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            total_sum += array[i][j];
        }
        printf("Thread %d processes row %d\n", omp_get_thread_num(), i);
    }
    printf("Suma elementów (wierszowa): %d\n", total_sum);

    // Reset sumy
    total_sum = 0;

    // Dekompocycja kolumnowa
    printf("\nSumowanie (dekompozycja kolumnowa):\n");
    #pragma omp parallel for reduction(+:total_sum) default(none) shared(array)
    for (int j = 0; j < COLS; j++) {
        for (int i = 0; i < ROWS; i++) {
            total_sum += array[i][j];
        }
        printf("Thread %d processes colum %d\n", omp_get_thread_num(), j);

    }
    printf("Suma elementów (kolumnowa): %d\n", total_sum);

    return 0;
}
