#include<stdlib.h>
#include<stdio.h>
#include<omp.h>

int main(){

#ifdef   _OPENMP
  printf("\nKompilator rozpoznaje dyrektywy OpenMP\n");
#endif

  int liczba_watkow;

  int a_shared = 1;
  int b_private = 2;
  int c_firstprivate = 3;
  int e_atomic=5;


  printf("przed wejsciem do obszaru rownoleglego -  nr_threads %d, thread ID %d\n",
         omp_get_num_threads(), omp_get_thread_num());
  printf("\ta_shared \t= %d\n", a_shared);
  printf("\tb_private \t= %d\n", b_private);
  printf("\tc_firstprivate \t= %d\n", c_firstprivate);
  printf("\te_atomic \t= %d\n", e_atomic);


#pragma omp parallel default(none) shared(a_shared, e_atomic) private(b_private) firstprivate(c_firstprivate )
  {
    int i;
   int d_local_private;


    d_local_private = a_shared + c_firstprivate; //1. WAR - a -1-2      RAW
   #pragma omp barrier
    for(i=0;i<10;i++){
#pragma omp atomic

      a_shared ++; //2. WAR - a-2       WAW     RAW
   }

    for(i=0;i<10;i++){
      c_firstprivate += omp_get_thread_num();
    }

    for(i=0;i<10;i++){
        #pragma omp atomic //3. WAR - e-3       RAW
      e_atomic+=omp_get_thread_num();
    }
    #pragma omp barrier
    #pragma omp critical
    {

      printf("\nw obszarze równoległym: aktualna liczba watkow %d, moj ID %d\n",
             omp_get_num_threads(), omp_get_thread_num());

      printf("\ta_shared \t= %d\n", a_shared); //4. RAW - a-2-4
      printf("\tb_private \t= %d\n", b_private);
      printf("\tc_firstprivate \t= %d\n", c_firstprivate);
      printf("\td_local_private = %d\n", d_local_private);
      printf("\te_atomic \t= %d\n", e_atomic); //5. RAW e-3-5

    }
  }

  printf("po zakonczeniu obszaru rownoleglego:\n");
  printf("\ta_shared \t= %d\n", a_shared);
  printf("\tb_private \t= %d\n", b_private);
  printf("\tc_firstprivate \t= %d\n", c_firstprivate);
  printf("\te_atomic \t= %d\n", e_atomic);

}

