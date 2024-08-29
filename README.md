# Projekat-Barbershop problem
##Opis problema:

Berbernica ima jednu berbersku stolicu i čekaonicu sa ograničenim brojem stolica (n).
Berberin radi samo kada ima mušterija. Ako nema mušterija, on zaspi.
Kada mušterija uđe:
Ako su sve stolice u čekaonici zauzete, mušterija napušta radnju.
Ako ima slobodnih stolica, mušterija seda i čeka da bude na redu.
Ako je berberin zaspao, mušterija ga budi.
Cilj problema je da se sinhronizuje ponašanje berberina i mušterija kako bi se izbegli konflikti, kao što su preklapanje operacija ili gubitak mušterija zbog loše organizacije.

##Objasnjenje koda
###POSIX semafori i mutex
sem_t cekaonica;   // Semafor za broj slobodnih stolica
sem_t barberChair;   // Semafor za frizersku stolicu
sem_t barberReady;   // Semafor za obaveštavanje frizera
pthread_mutex_t mutex; // Mutex za pristup deljenim resursima


