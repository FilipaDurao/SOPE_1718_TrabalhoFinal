all: client

client: log.o
	gcc -Wall -Wextra -g client.c log.o -o client

log.o:
	gcc -Wall -Wextra -g -c log.c -o log.o 

clean:
	rm -f client
	rm -f *.o
	