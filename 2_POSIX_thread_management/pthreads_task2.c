#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


void *watek_fun(void *arg) {
    int id = *((int *)arg); // id watku
    pthread_t tid = pthread_self(); // systemowe id watku


    printf("systemowe id watku: %lu, przekazane id: %d\n", (unsigned long)tid, id);

    return NULL;
}

int main() {
    int max_watkow = 5;
    pthread_t tablica_id_watki[max_watkow]; // do id watkow
    int *ids[max_watkow]; // do wskaznikow do id (po to, by zapewnic unikalnosc podczas wspolbieznosci)
    int i, liczba_watkow;

    for (liczba_watkow = 1; liczba_watkow <= max_watkow; liczba_watkow++) {
        printf("tworzenie %d wątków:\n", liczba_watkow);

        // tworzenie
        for (i = 0; i < liczba_watkow; i++) {
            ids[i] = malloc(sizeof(int)); // alokacja
            *ids[i] = i;
            pthread_create(&tablica_id_watki[i], NULL, watek_fun, (void *)ids[i]); // <- tu tworzenie
        }

        // czekanie na skocznczenie n watkow
        for (i = 0; i < liczba_watkow; i++) {
            pthread_join(tablica_id_watki[i], NULL);
            free(ids[i]); // pamiec
        }

        printf("wszystkie watki sie skonczyly.\n\n");
    }


        // BLAD SYNCHORNIZACJI
        printf("\n\tBLAD SYNCHROZNIACJI\n");
for (liczba_watkow = 1; liczba_watkow <= max_watkow; liczba_watkow++) {

        for (i = 0; i < liczba_watkow; i++) {
            // tutaj przekazujemy adres zmiennej i - blad
            pthread_create(&tablica_id_watki[i], NULL, watek_fun, (void *)&i);
        }

        for (i = 0; i < liczba_watkow; i++) {
            pthread_join(tablica_id_watki[i], NULL);
        }


    }
    return 0;
}
