#include<stdlib.h>
#include<stdio.h>
#include<omp.h>

int f_threadprivate = 3;
#pragma omp threadprivate(f_threadprivate)


int main(){

#ifdef   _OPENMP
  printf("\nKompilator rozpoznaje dyrektywy OpenMP\n");
#endif

  int liczba_watkow;

  int a_shared = 1;
  int b_private = 2;
  int c_firstprivate = 3;
  int e_atomic=5;

omp_set_num_threads(5);


  printf("przed wejsciem do obszaru rownoleglego -  nr_threads %d, thread ID %d\n",
         omp_get_num_threads(), omp_get_thread_num());
  printf("\ta_shared \t= %d\n", a_shared);
  printf("\tb_private \t= %d\n", b_private);
  printf("\tc_firstprivate \t= %d\n", c_firstprivate);
  printf("\te_atomic \t= %d\n", e_atomic);


#pragma omp parallel shared(a_shared, e_atomic) private(b_private) firstprivate(c_firstprivate )
  {
    int i;
    int d_local_private;

   f_threadprivate = omp_get_thread_num();

        #pragma omp barrier
    d_local_private = a_shared + c_firstprivate; //niżej modyfikuje się a - ryzyko niespójności - zależność RAW zeliminowana
   #pragma omp barrier                  //też eliminacja WAR - ryzyko modyfikacji a przed końcem tego
    for(i=0;i<10;i++){
#pragma omp atomic

      a_shared ++; //elimicjacja WAW - ryzyko wyścigów
   }

    for(i=0;i<10;i++){
      c_firstprivate += omp_get_thread_num();
    }

    for(i=0;i<10;i++){
        #pragma omp atomic //eliminacja WAW
      e_atomic+=omp_get_thread_num();
    }
#pragma omp critical
{

      printf("\nw obszarze równoległym: aktualna liczba watkow %d, moj ID %d\n",
             omp_get_num_threads(), omp_get_thread_num());

      printf("\ta_shared \t= %d\n", a_shared);
      printf("\tb_private \t= %d\n", b_private);
      printf("\tc_firstprivate \t= %d\n", c_firstprivate);
      printf("\td_local_private = %d\n", d_local_private);
      printf("\te_atomic \t= %d\n", e_atomic);
      printf("\tf_threadprivate \t= %d\n", f_threadprivate);
    }
  }

  printf("po zakonczeniu obszaru rownoleglego:\n");
  printf("\ta_shared \t= %d\n", a_shared);
  printf("\tb_private \t= %d\n", b_private);
  printf("\tc_firstprivate \t= %d\n", c_firstprivate);
  printf("\te_atomic \t= %d\n", e_atomic);



//DRUGI OBSZAR RÓWNOLEGŁY
#pragma omp parallel shared (a_shared)
{
        #pragma omp critical
{
        printf("\n\nwatek w drugim obszare rownoleglym nr: %d\n", omp_get_thread_num());
        printf("\t f_threadprivate = %d\n", f_threadprivate);
}
}
