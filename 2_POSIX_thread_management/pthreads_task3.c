#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


typedef struct {
    double start;   // pkt poczatkowy fragmentu
    double end;     // pktkt koncowy fragmentu
    double result;  // wynik in/out
    int thread_id;  // id wtku in
} ThreadData;


double funkcja(double x) {
    return x * x;
}


void *calka(void *arg) {
    ThreadData *data = (ThreadData *)arg;

    double srodek = (data->start + data->end) / 2.0;
    data->result = funkcja(srodek);

    printf("id watku %d: start fragmentu = %.2f, end fragmentu = %.2f, wynik = %.2f\n", data->thread_id, data->start, data->end, dat>
    return NULL;
}

int main() {
    int liczba_watkow = 2;
    pthread_t watki[liczba_watkow];
    ThreadData dane[liczba_watkow]; // tab lokalnych struktur
    ThreadData *dane_ptr; // wskaznik do struktury dla drugiego watu

    double a = 0.0, b = 10.0;
    double przedzial = (b - a) / liczba_watkow;
    double wynik_ostat = 0.0;

    // watek nr1 - obiekt lokalny struktury
    dane[0].start = a;
    dane[0].end = a + przedzial;
    dane[0].result = 0.0;
    dane[0].thread_id = 1; //id

    pthread_create(&watki[0], NULL, calka, (void *)&dane[0]);

    // watek nr2 - wskaxnik do struktury
    dane_ptr = malloc(sizeof(ThreadData)); // alokacja
    dane_ptr->start = dane[0].end;
    dane_ptr->end = b;
    dane_ptr->result = 0.0;
    dane_ptr->thread_id = 2; // id
    pthread_create(&watki[1], NULL, calka, (void *)dane_ptr);


    for (int i = 0; i < liczba_watkow; i++) {
        pthread_join(watki[i], NULL);
    }


    wynik_ostat = dane[0].result + dane_ptr->result;

    printf("wynik: %.2f\n", wynik_ostat);


    free(dane_ptr);

    return 0;
}
