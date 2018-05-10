all: client

client: 
	gcc -Wall -Wextra client.c -o client

log.o:
	gcc -Wall -Wextra -c log.c -o log.o 

clean:
	rm -f client
	rm -f *.o
	