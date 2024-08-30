#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_BROJ_MUSTERIJA 10
#define BROJ_STOLICA 3

// Globalni POSIX semafori i mutex
sem_t waitingRoom;   // Semafor za broj slobodnih stolica
sem_t barberChair;   // Semafor za frizersku stolicu
sem_t barberReady;   // Semafor za obaveštavanje frizera
sem_t done;           // Semafor za kraj
pthread_mutex_t mutex; // Mutex za pristup deljenim resursima

int cekanje = 0;

// Funkcija za frizera
void* barberT(void* arg) {
    while (1) {
        // Proverava da li treba da završi rad
        if (sem_trywait(&done) == 0) {
            printf("Frizer: Završava rad.\n");
            break; // Izlazi iz petlje i završava rad
        }
        // Čeka klijenta da se pojavi
        sem_wait(&barberReady);

        // Izvršava friziranje
        pthread_mutex_lock(&mutex);
        cekanje--;
        printf("Frizer: Počeo šišanje klijenta. Broj klijenata koji čekaju: %d\n", cekanje);
        pthread_mutex_unlock(&mutex);

        
        sleep(3);  // Simulacija vremena friziranja

        // Oslobađa stolicu za sledećeg klijenta
        sem_post(&barberChair);
    }
    return NULL;
}

// Funkcija za klijenta
void* customerT(void* arg) {
    int id = (intptr_t)arg;

    // Pokušava da zauzme mesto u čekaonici
    if (sem_trywait(&waitingRoom) == 0) {
        pthread_mutex_lock(&mutex);
        cekanje++;
        printf("Klijent %d: Ušao u čekaonicu. Broj čekanja: %d\n", id, cekanje);
        pthread_mutex_unlock(&mutex);

        // Signalizira frizeru da je spreman
        sem_post(&barberReady);

        // Čeka da frizer završi
        sem_wait(&barberChair);
        printf("Klijent %d: Započeo šišanje.\n", id);

        
        printf("Klijent %d: Završio šišanje i odlazi.\n", id);

        // Oslobađa mesto u čekaonici
        sem_post(&waitingRoom);
    } else {
        printf("Klijent %d: Čekaonica puna. Odlazi.\n", id);
    }

    return NULL;
}

int main() {
    pthread_t barber;
    pthread_t musterije[MAX_BROJ_MUSTERIJA];

    // Inicijalizacija semafora i mutex-a
    sem_init(&waitingRoom, 0, BROJ_STOLICA);
    sem_init(&barberChair, 0, 1);
    sem_init(&barberReady, 0, 0);
    pthread_mutex_init(&mutex, NULL);
    sem_init(&done, 0, 0);

    // Kreiranje niti za frizera
    pthread_create(&barber, NULL, barberT, NULL);

    // Kreiranje niti za klijente
    for (int i = 0; i < MAX_BROJ_MUSTERIJA; i++) {
        pthread_create(&musterije[i], NULL, customerT, (void *)(intptr_t)(i + 1));
        sleep(1);  // Klijenti dolaze u regularnim intervalima
    }

    // Čeka da se sve niti završe
    for (int i = 0; i < MAX_BROJ_MUSTERIJA; i++) {
        pthread_join(musterije[i], NULL);
    }

    // Prekida rad frizera 
    sem_post(&done);
    pthread_join(barber, NULL);

    // Uništava semafore i mutex
    sem_destroy(&waitingRoom);
    sem_destroy(&barberChair);
    sem_destroy(&barberReady);
    pthread_mutex_destroy(&mutex);
    sem_destroy(&done);

    printf("\nKraj programa\n");
    return 0;
}