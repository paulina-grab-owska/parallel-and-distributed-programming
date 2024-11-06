#include<stdio.h>
#include<pthread.h>
#include<math.h>

double funkcja ( double x );

double calka_zrownoleglenie_petli(double a, double b, double dx, int l_w);

static int l_w_global=0;

static double calka_global=0.0;
static double a_global;
static double b_global;
static double dx_global;
static int N_global;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // do synchronizacji dostępu do calka_global

void* calka_fragment_petli_w(void* arg_wsk);

double calka_zrownoleglenie_petli(double a, double b, double dx, int l_w){

  int N = ceil((b-a)/dx);
  double dx_adjust = (b-a)/N;

  printf("Obliczona liczba trapezów: N = %d, dx_adjust = %lf\n", N, dx_adjust);
  //printf("a %lf, b %lf, n %d, dx %.12lf (dx_adjust %.12lf)\n", a, b, N, dx, dx_adjust);


  // tworzenie struktur danych do obsługi wielowątkowości

  a_global = a;
  b_global = b;
  dx_global = dx_adjust;
  N_global = N;
  l_w_global = l_w;

  // tworzenie wątków
  pthread_t watki[l_w];
  int ids[l_w]; // identyfikatory wątków

  for (int i = 0; i < l_w; i++) {
      ids[i] = i; // id wątku
      pthread_create(&watki[i], NULL, calka_fragment_petli_w, &ids[i]);
  }

  // oczekiwanie na zakończenie pracy wątków
  for (int i = 0; i < l_w; i++) {
      pthread_join(watki[i], NULL);
  }


  return(calka_global);
}

void* calka_fragment_petli_w(void* arg_wsk){

    int my_id = *(int*)arg_wsk; // pobranie id wątku

    double a = a_global;
    double dx = dx_global;
    int N = N_global;
    int l_w = l_w_global;

  // dekompozycja cykliczna
/*    int my_start = my_id;
    int my_stride = l_w;
    int my_end = N;
*/
  // dekompozycja blokowa
  int my_start = my_id * (N / l_w);
  int my_end = (my_id == l_w - 1) ? N : my_start + (N / l_w);
  int my_stride = 1;

  // something else ? (dekompozycja blokowo-cykliczna)

    printf("\nWątek %d: my_start %d, my_end %d, my_stride %d\n", my_id, my_start, my_end, my_stride);



  double calka = 0.0;
  for (int i = my_start; i < my_end; i += my_stride) {
      double x1 = a + i * dx;
      calka += 0.5 * dx * (funkcja(x1) + funkcja(x1 + dx));
  }

  // Dodawanie wyniku wątku do globalnej zmiennej całki
  pthread_mutex_lock(&mutex);
  calka_global += calka;
  pthread_mutex_unlock(&mutex);

  return NULL;

}
                                                       
