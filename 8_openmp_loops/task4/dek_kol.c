#include <stdio.h>
#include <omp.h>

#define ROWS 10
#define COLS 10

int main() {
    int array[ROWS][COLS];
    int total_sum = 0;

        //tablica
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            array[i][j] = i + j;
        }
    }

    printf("Tablica 2D:\n");
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%3d ", array[i][j]);
        }
        printf("\n");
    }

    // tablica na sumy lokalne
    int local_sums[COLS] = {0}; // jedna suma - jedna komlumna

    // Dekompocycja kolumnowa
    printf("\nSumowanie (dekompozycja kolumnowa, bez reduction):\n");
    #pragma omp parallel default(none) shared(array, local_sums)
    {
        int thread_id = omp_get_thread_num();
        double sum_loc =0;
        // wątek - lokalne sumy dla kolumn
        #pragma omp for
        for (int j = 0; j < COLS; j++) {
            for (int i = 0; i < ROWS; i++) {
                //#pragma omp critical
                //local_sums[j] += array[i][j];
                sum_loc += array[i][j];
}
        }
//*pragma critical
    }

    // suming
    for (int j = 0; j < COLS; j++) {
        total_sum += local_sums[j];
    }

    printf("Suma elementów (kolumnowa): %d\n", total_sum);

    return 0;
}
                          
