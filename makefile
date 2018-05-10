all: client

client: log.o
	gcc -Wall -Wextra client.c log.o -o client

log.o:
	gcc -Wall -Wextra -c log.c -o log.o 

clean:
	rm -f client
	rm -f *.o
	