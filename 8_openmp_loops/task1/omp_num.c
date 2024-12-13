#include <stdio.h>
#include <omp.h>

int main() {

    #pragma omp parallel default(none)
    {
        int thread_id = omp_get_thread_num();
        int total_threads = omp_get_num_threads();
        #pragma omp critical
        {
            printf("Hello from thread %d out of %d\n", thread_id, total_threads);
        }
    }

    return 0;
}
//export OMP_NUM_THREADS=8
