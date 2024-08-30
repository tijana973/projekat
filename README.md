# Projekat-Barbershop problem
## Opis problema:

Berbernica ima jednu berbersku stolicu i čekaonicu sa ograničenim brojem stolica (n).
Berberin radi samo kada ima mušterija. Ako nema mušterija, on zaspi.
Kada mušterija uđe:
Ako su sve stolice u čekaonici zauzete, mušterija napušta radnju.
Ako ima slobodnih stolica, mušterija seda i čeka da bude na redu.
Ako je berberin zaspao, mušterija ga budi.

Cilj problema je da se sinhronizuje ponašanje berberina i mušterija kako bi se izbegli konflikti, kao što su preklapanje operacija ili gubitak mušterija zbog loše organizacije.

## Objasnjenje koda
### POSIX semafori i mutex

    sem_t cekaonica;   // Semafor za broj slobodnih stolica
    sem_t barberChair;   // Semafor za frizersku stolicu
    sem_t barberReady;   // Semafor za obaveštavanje frizera
    sem_t done;    //Semafor za kraj
    pthread_mutex_t mutex; // Mutex za pristup deljenim resursima
    int cekanje; // Broj klijenata koji čekaju

### Funkcije

Funkcija niti za frizera
void* barberT(void* arg):  Frizer čeka na klijenta, šiša ga i oslobađa frizersku stolicu.

Funkcija niti za klijenta
void* customerT(void* arg):  Klijent pokušava da zauzme mesto u čekaonici; ako uspe, signalizira frizeru i čeka na šišanje.

Glavna funkcija 
int main(): Inicijalizuje semafore i mutex, kreira niti za frizera i klijente, i čeka da se sve niti završe.

### Ostale promenljive

MAX_BROJ_MUSTERIJA: Ukupan broj klijenata koji dolaze u salon.

BROJ_STOLICA: Broj stolica u čekaonici.

Vremenski interval dolaska klijenata (sleep(1) u main funkciji).

Vreme šišanja (sleep(3) u funkciji barberT).

### Main funkcija
 //Kreiranje niti
 
    pthread_t barber;
    pthread_t musterije[MAX_BROJ_MUSTERIJA];

 // Inicijalizacija semafora i mutex-a
 
       sem_init(&waitingRoom, 0, BROJ_STOLICA);
       sem_init(&barberChair, 0, 1);
       sem_init(&barberReady, 0, 0);
       sem_init(&done, 0, 0);
       pthread_mutex_init(&mutex, NULL);

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
    sem_destroy(&done);
    pthread_mutex_destroy(&mutex);
//Kraj 

    printf("\nKraj programa\n"); 
    return 0;


