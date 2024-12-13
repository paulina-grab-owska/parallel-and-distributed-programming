#include <stdio.h>
#include <omp.h>

int main() {
    #pragma omp parallel num_threads(4) default(none)
    {
        printf("Hello from thread %d out of %d\n", omp_get_thread_num(), omp_get_num_threads());
    }
    return 0;
}
