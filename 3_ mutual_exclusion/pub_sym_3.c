#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_TAPS 3  // liczba kranów
#define NUM_MUGS 5  // liczba kufli

//  kran
typedef struct {
    char *beer_name;
    pthread_mutex_t tap_mutex;  // muteks dostepu do kranu
} Tap;

// pub
typedef struct {
    Tap taps[NUM_TAPS];      // krany
    int available_mugs;      // liczba dostepnych kufli
    pthread_mutex_t mug_mutex;  // muteks do kufli
} Pub;

//  klient - watek
void* client_routine(void* arg) {
    Pub* pub = (Pub*)arg;
    int mugs_needed = 2;  // liczba kufli dla klienta
    long int work_done = 0;

    for (int i = 0; i < mugs_needed; i++) {
        // pobranie kufla
        while (1) {
            if (pthread_mutex_trylock(&pub->mug_mutex) == 0) {
                if (pub->available_mugs > 0) {
                    pub->available_mugs--;
                    printf("klient %ld pobrał kufel. \t pozostało kufli: %d\n", pthread_self(), pub->available_mugs);
                    pthread_mutex_unlock(&pub->mug_mutex);
                    break;  // udało się pobrać kufel
                } else {
                    pthread_mutex_unlock(&pub->mug_mutex);
                    usleep(100);  // aktywne czekanie
                }
            }
            else {
                work_done++;  // aktywne czekanie - praca
            }
        }

        // pobranie kranu
        int chosen_tap = -1;
        while (chosen_tap == -1) {
            for (int j = 0; j < NUM_TAPS; j++) {
                if (pthread_mutex_trylock(&pub->taps[j].tap_mutex) == 0) {
                    chosen_tap = j;
                    printf("klient %ld nalewa piwo z kranu %s.\n", pthread_self(), pub->taps[j].beer_name);
                    pthread_mutex_unlock(&pub->taps[j].tap_mutex);
                    break;
                }
            }
            if (chosen_tap == -1) {
                usleep(100);  // aktywne czekanie
            }
            work_done++;
        }

        // picie
        printf("klient %ld pije piwo z kranu %s.\n", pthread_self(), pub->taps[chosen_tap].beer_name);
        sleep(2);  // czas picia

        // oddanie kufla
        pthread_mutex_lock(&pub->mug_mutex);
        pub->available_mugs++;
        printf("klient %ld oddal kufel. \t dostepnych kufli: %d\n", pthread_self(), pub->available_mugs);
        pthread_mutex_unlock(&pub->mug_mutex);
    }

    printf("klient %ld: wykonana %ld praca w oczekiwaniuna kufel/kran.\n", pthread_self(), work_done);
    return NULL;
}

int main() {
    Pub pub;

    // krany
    pub.taps[0].beer_name = "Pilsner";
    pub.taps[1].beer_name = "Tyskie";
    pub.taps[2].beer_name = "Cydr";

    for (int i = 0; i < NUM_TAPS; i++) {
        pthread_mutex_init(&pub.taps[i].tap_mutex, NULL);
    }

    // kufle
    pub.available_mugs = NUM_MUGS;
    pthread_mutex_init(&pub.mug_mutex, NULL);

    // klienci
    int num_clients = 6;
    pthread_t clients[num_clients];

    for (int i = 0; i < num_clients; i++) {
        pthread_create(&clients[i], NULL, client_routine, (void*)&pub);
    }

    // czekanie na skonczenie
    for (int i = 0; i < num_clients; i++) {
        pthread_join(clients[i], NULL);
    }

    // czyszczenie mutexów
    for (int i = 0; i < NUM_TAPS; i++) {
        pthread_mutex_destroy(&pub.taps[i].tap_mutex);
    }
    pthread_mutex_destroy(&pub.mug_mutex);

    printf("pub zamyka się. \t zwrocono wszystkie kufle\n");

    return 0;
}
