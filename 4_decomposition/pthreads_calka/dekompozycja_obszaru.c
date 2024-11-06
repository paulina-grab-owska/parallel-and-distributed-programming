#include<stdio.h>
#include<pthread.h>
#include<math.h>

double funkcja ( double x );

double calka_dekompozycja_obszaru(double a, double b, double dx, int l_w);

double calka_sekw(double a, double b, double dx);

void* calka_podobszar_w(void* arg_wsk);

typedef struct {
    double a;        // lewy kraniec podobszaru
    double b;        // prawy kraniec podobszaru
    double dx;      // h trapezu
    double result;  // wynik w podobszarze
} Podobszar;


double calka_dekompozycja_obszaru(double a, double b, double dx, int l_w){

    // tablica wskaźników do wątków
    pthread_t* wątki = malloc(l_w * sizeof(pthread_t));
    Podobszar* podobszary = malloc(l_w * sizeof(Podobszar));

    double calka_suma_local = 0.0;
    double dlugosc = (b - a) / l_w;

  // tworzenie struktur danych do obsługi wielowątkowości


  // tworzenie wątków
    for (int i = 0; i < l_w; i++) {
        podobszary[i].a = a + i * dlugosc;
        podobszary[i].b = a + (i + 1) * dlugosc;
        podobszary[i].dx = dx;
        podobszary[i].result = 0.0;

        pthread_create(&wątki[i], NULL, calka_podobszar_w, (void*)&podobszary[i]);
    }

  // oczekiwanie na zakończenie pracy wątków
    for (int i = 0; i < l_w; i++) {
        pthread_join(wątki[i], NULL);
        calka_suma_local += podobszary[i].result;
        }
    free(wątki);
    free(podobszary);

  return(calka_suma_local);
}



void* calka_podobszar_w(void* arg_wsk){

    Podobszar* podobszar = (Podobszar*)arg_wsk;

    double a_local = podobszar->a;
    double b_local = podobszar->b;
    double dx = podobszar->dx;

     // Obliczanie całki w podobszarze
    podobszar->result = calka_sekw(a_local, b_local, dx);

    printf("\nWątek: a_local %lf, b_local %lf, dx %lf, wynik = %lf\n",
           a_local, b_local, dx, podobszar->result);

    return NULL;

}
