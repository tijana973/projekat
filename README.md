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


