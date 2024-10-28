#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define LITR_TO_DRINK 3  // liczba kufli na klienta

int available_mugs;
pthread_mutex_t mug_mutex;

// klient-watek
void* client_thread(void* arg) {
    int client_id = *(int*)arg;
    int mugs_drunk = 0;
    long int wykonana_praca = 0;  // praca podczas aktywnego czekania

    while (mugs_drunk < LITR_TO_DRINK) {
        //proba pobrania kufla z trylockiem
        if (pthread_mutex_trylock(&mug_mutex) == 0) {
            if (available_mugs > 0) {
                available_mugs--;  // pobranie
                printf("klient %d pobral kufel. \t pozostalo kufli: %d\n", client_id, available_mugs);
                pthread_mutex_unlock(&mug_mutex);

                // napelnianie
                printf("klient %d napelnia kufel.\n", client_id);
                sleep(rand() % 3 + 1);  // kilka sekund

                // picie
                printf("klient %d pije piwo.\n", client_id);
                sleep(rand() % 5 + 5);  //  kilkanaccie sekund

                // oddanie kufla
                pthread_mutex_lock(&mug_mutex);
                available_mugs++;
                printf("klient %d oddal kufel. \t dostepnych kufli: %d\n", client_id, available_mugs);
                pthread_mutex_unlock(&mug_mutex);

                mugs_drunk++;
            } else {
                // brak kufli unlock mutex
                pthread_mutex_unlock(&mug_mutex);
            }
        } else {
            // aktywne czekanie
            wykonana_praca++;
            printf("klient %d pracuje w oczekiwaniu.\n", client_id);
            usleep(50000);  // symulowacja pracy
        }
    }

    printf("klient %d wypil %d kufli piwa i opuszcza pub. \t wykonana praca: %ld\n", client_id, mugs_drunk, wykonana_praca);
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s <liczba_kufli> <liczba_klientow>\n", argv[0]);
        return -1;
    }

    int l_kf = atoi(argv[1]);
    int l_kl = atoi(argv[2]);

    available_mugs = l_kf;

    pthread_t clients[l_kl];
    int client_ids[l_kl];

    // inicjalizacja mutexa
    pthread_mutex_init(&mug_mutex, NULL);

    //tworzenie watkow (klientow)
    for (int i = 0; i < l_kl; i++) {
        client_ids[i] = i + 1;
        pthread_create(&clients[i], NULL, client_thread, &client_ids[i]);
    }

    // czekanie na skonczenie
    for (int i = 0; i < l_kl; i++) {
        pthread_join(clients[i], NULL);
    }

    // czy liczba kufli jest taka jaka byla
    if (available_mugs == l_kf) {
        printf("symulacja zakończona poprawnie. wszystkie kufle oddano.\n");
    } else {
        printf("ERROR: liczba kufli sie nie zgadza!\n");
    }

    // zwalnianie zasobow
    pthread_mutex_destroy(&mug_mutex);

    return 0;
}
