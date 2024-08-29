#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_BROJ_MUSTERIJA 10
#define BROJ_STOLICA 3


sem_t cekaonica;   
sem_t barberChair;   
sem_t barberReady;   
pthread_mutex_t mutex; 

int vreme=0;
void* barberT(void* arg) {
    while (1) {
        // Čeka klijenta da se pojavi
        sem_wait(&barberReady);

        // Izvršava friziranje
        pthread_mutex_lock(&mutex);
        vreme--;
        printf("Frizer: Počeo friziranje klijenta. Vreme čekanja: %d\n", vreme);
        pthread_mutex_unlock(&mutex);

        // Klijent je friziran
        sleep(3);  // Simulacija vremena friziranja

        // Oslobađa stolicu za sledećeg klijenta
        sem_post(&barberChair);
    }
    return NULL;
}


void* customerT(void* arg) {
    int id = (intptr_t)arg;

    // Pokušava da zauzme mesto u čekaonici
    if (sem_trywait(&cekaonica) == 0) {
        pthread_mutex_lock(&mutex);
        vreme++;
        printf("Klijent %d: Ušao u čekaonicu. Vreme čekanja: %d\n", id, vreme);
        pthread_mutex_unlock(&mutex);

        // Signalizira frizeru da je spreman
        sem_post(&barberReady);

        // Čeka da frizer završi
        sem_wait(&barberChair);
        printf("Klijent %d: Započeo friziranje.\n", id);

        // Odlazi nakon friziranja
        printf("Klijent %d: Završio friziranje i odlazi.\n", id);

        // Oslobađa mesto u čekaonici
        sem_post(&cekaonica);
    } else {
        printf("Klijent %d: Čekaonica puna. Odlazi.\n", id);
    }

    return NULL;
}

int main() {
    pthread_t barber;
    pthread_t musterije[MAX_BROJ_MUSTERIJA];

    
    sem_init(&cekaonica, 0, BROJ_STOLICA);
    sem_init(&barberChair, 0, 1);
    sem_init(&barberReady, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    
    pthread_create(&barber, NULL, barberT, NULL);

    
    for (int i = 0; i < MAX_BROJ_MUSTERIJA; i++) {
        pthread_create(&musterije[i], NULL, customerT, (void *)(intptr_t)(i + 1));
        sleep(1);  
    }

    
    for (int i = 0; i < MAX_BROJ_MUSTERIJA; i++) {
        pthread_join(musterije[i], NULL);
    }

    
    pthread_cancel(barber);
    pthread_join(barber, NULL);

    
    sem_destroy(&cekaonica);
    sem_destroy(&barberChair);
    sem_destroy(&barberReady);
    pthread_mutex_destroy(&mutex);

    printf("\nKraj programa\n");
    return 0;
}