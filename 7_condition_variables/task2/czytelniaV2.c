#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

#include"czytelnia.h"


pthread_mutex_t mutex;
pthread_cond_t czytelnicy;
pthread_cond_t pisarze;

/*** Implementacja procedur interfejsu ***/

int my_read_lock_lock(cz_t* cz_p){

  pthread_mutex_lock(&mutex);

    while (cz_p->l_p > 0) {
        pthread_cond_wait(&czytelnicy, &mutex);
    }

    cz_p->l_c++;  // Zwiększenie liczby czytelników

    pthread_mutex_unlock(&mutex);
    return 0;

}
int my_read_lock_unlock(cz_t* cz_p){

  pthread_mutex_lock(&mutex);

    cz_p->l_c--;  // Zmniejszenie liczby czytelników

    if (cz_p->l_c == 0) {
        pthread_cond_signal(&pisarze);  // Obudzenie pisarza, jeśli żaden czytelnik nie jest aktywny
    }

    pthread_mutex_unlock(&mutex);
    return 0;

}


int my_write_lock_lock(cz_t* cz_p){

  pthread_mutex_lock(&mutex);

    while (cz_p->l_c > 0 || cz_p->l_p > 0) {
        pthread_cond_wait(&pisarze, &mutex);
    }

    cz_p->l_p++;  // Zwiększenie liczby pisarzy

    pthread_mutex_unlock(&mutex);
    return 0;
}


int my_write_lock_lock(cz_t* cz_p){

  pthread_mutex_lock(&mutex);

    while (cz_p->l_c > 0 || cz_p->l_p > 0) {
        pthread_cond_wait(&pisarze, &mutex);
    }

    cz_p->l_p++;  // Zwiększenie liczby pisarzy

    pthread_mutex_unlock(&mutex);
    return 0;

}


int my_write_lock_unlock(cz_t* cz_p){

  pthread_mutex_lock(&mutex);

    cz_p->l_p--;  // Zmniejszenie liczby pisarzy

    if (cz_p->l_p == 0) {
        pthread_cond_broadcast(&czytelnicy);  // Obudzenie czekających czytelników
    } else {
        pthread_cond_signal(&pisarze);  // Obudzenie pisarza, jeśli jest
    }

    pthread_mutex_unlock(&mutex);
    return 0;
}

void inicjuj(cz_t* cz_p){

  cz_p->l_p = 0;
  cz_p->l_c = 0;

}

void czytam(cz_t* cz_p){

// wypisanie wartości zmiennych kontrolujących działanie: liczby czytelników i pisarzy
  printf("\t\t\t\t\tczytam:  l_c %d, l_p %d\n", cz_p->l_c, cz_p->l_p);
// sprawdzenie warunku poprawności i ewentualny exit
// warunek:
  if( cz_p->l_p>1 || (cz_p->l_p==1 && cz_p->l_c>0) || cz_p->l_p<0 || cz_p->l_c<0 ) {
    printf("Błąd: liczba pisarzy lub czytelników jest nieprawidłowa!\n");
        printf("l_p: %d, l_c: %d\n", cz_p->l_p, cz_p->l_c);
        exit(0);

  }

    usleep(rand()%3000000);
}

void pisze(cz_t* cz_p){

// wypisanie wartości zmiennych kontrolujących działanie: liczby czytelników i pisarzy
  printf("\t\t\t\t\tpisze:   l_c %d, l_p %d\n", cz_p->l_c, cz_p->l_p);
// sprawdzenie warunku poprawności i ewentualny exit
  if( cz_p->l_p>1 || (cz_p->l_p==1 && cz_p->l_c>0) || cz_p->l_p<0 || cz_p->l_c<0 ) {
            printf("Błąd: liczba pisarzy lub czytelników jest nieprawidłowa!\n");
        printf("l_p: %d, l_c: %d\n", cz_p->l_p, cz_p->l_c);
    exit(0);
  }

    usleep(rand()%3000000);
}


