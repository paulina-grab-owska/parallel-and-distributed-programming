#include <stdio.h>
#include <omp.h>

int main() {
    omp_set_num_threads(4); // Ustaw liczbę wątków

    #pragma omp parallel default(none)
    {
        printf("Hello from thread %d out of %d\n", omp_get_thread_num(), omp_get_num_threads());
    }
    return 0;
}
