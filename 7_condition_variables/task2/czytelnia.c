#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "czytelnia.h"

void inicjuj(cz_t* cz_p) {
    cz_p->l_p = 0;
    cz_p->l_c = 0;
    cz_p->waiting_readers = 0;
    cz_p->waiting_writers = 0;
    pthread_mutex_init(&cz_p->mutex, NULL);
    pthread_cond_init(&cz_p->readers, NULL);
    pthread_cond_init(&cz_p->writers, NULL);
}

void czytam(cz_t* cz_p) {
    printf("\t\t\tczytam: l_c %d, l_p %d\n", cz_p->l_c, cz_p->l_p);
#ifdef MY_DEBUG
    if (cz_p->l_p > 0 || cz_p->l_c < 0) {
        printf("Błąd: l_c: %d, l_p: %d\n", cz_p->l_c, cz_p->l_p);
        exit(1);
    }
#endif
    usleep(rand() % 3000000);
}

void pisze(cz_t* cz_p) {
    printf("\t\t\tpiszę: l_c %d, l_p %d\n", cz_p->l_c, cz_p->l_p);
#ifdef MY_DEBUG
    if (cz_p->l_c > 0 || cz_p->l_p != 1) {
        printf("Błąd: l_c: %d, l_p: %d\n", cz_p->l_c, cz_p->l_p);
        exit(1);
    }
#endif
    usleep(rand() % 3000000);
}

int my_read_lock_lock(cz_t* cz_p) {
    pthread_mutex_lock(&cz_p->mutex);
    if (cz_p->l_p > 0 || cz_p->waiting_writers>0) {
        cz_p->waiting_readers++;
        pthread_cond_wait(&cz_p->readers, &cz_p->mutex);
        cz_p->waiting_readers--;
    }
    cz_p->l_c++;
    pthread_mutex_unlock(&cz_p->mutex);
    return 0;
}

int my_read_lock_unlock(cz_t* cz_p) {
    pthread_mutex_lock(&cz_p->mutex);
    cz_p->l_c--;
    if (cz_p->l_c == 0 && cz_p->waiting_writers > 0) {
        pthread_cond_signal(&cz_p->writers);
    }
    pthread_mutex_unlock(&cz_p->mutex);
    return 0;
}

int my_write_lock_lock(cz_t* cz_p) {
    pthread_mutex_lock(&cz_p->mutex);

    //if(cz_p->l_p == 1)
    //pthread_cond_wa
    if (cz_p->l_p > 0 || cz_p->l_c > 0) {
        cz_p->waiting_writers++;
        pthread_cond_wait(&cz_p->writers, &cz_p->mutex);
        cz_p->waiting_writers--;
    }
    cz_p->l_p++;
    pthread_mutex_unlock(&cz_p->mutex);
    return 0;
}

int my_write_lock_unlock(cz_t* cz_p) {
    pthread_mutex_lock(&cz_p->mutex);
    cz_p->l_p--;
    if (cz_p->waiting_writers > 0) {
        pthread_cond_broadcast(&cz_p->readers);
    } else if (cz_p->waiting_readers > 0) {
        pthread_cond_signal(&cz_p->writers);
    }
    pthread_mutex_unlock(&cz_p->mutex);
    return 0;
}
