
all: barber 

barber: main.c
	gcc -o barber main.c -lpthread

clean:
	rm -f barber
