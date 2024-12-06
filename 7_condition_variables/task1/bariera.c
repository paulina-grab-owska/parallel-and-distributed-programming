#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

static int liczba_watkow;
static int liczba_watkow_przeszla;
 pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;


void bariera_init(int n) {
    liczba_watkow = n;
    liczba_watkow_przeszla = 0;
}


void bariera(void) {
    pthread_mutex_lock(&mutex);

    liczba_watkow_przeszla++;


    if (liczba_watkow_przeszla == liczba_watkow) {
        liczba_watkow_przeszla = 0;
        pthread_cond_broadcast(&cond);
    } else {
        while (liczba_watkow_przeszla < liczba_watkow) {
            pthread_cond_wait(&cond, &mutex);
 }   }

    pthread_mutex_unlock(&mutex);

}

void  barierav2(void){


    pthread_mutex_lock(&mutex);

    liczba_watkow_przeszla++;


    if (liczba_watkow_przeszla == liczba_watkow) {

        liczba_watkow_przeszla = 0;
        pthread_cond_broadcast(&cond);
    } else {

            pthread_cond_wait(&cond, &mutex);

}
    pthread_mutex_unlock(&mutex);
}
