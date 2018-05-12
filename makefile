all: client

client: log.o
	gcc -Wall -Wextra -g client.c log.o -o client

log.o:
	gcc -Wall -Wextra -g -c log.c -o log.o 

clean:
	rm -f client
	rm -f *.o
	
room.o:
	gcc -Wall -Wextra -c room.c -o room.o
server: room.o
	gcc -Wall -Wextra -lpthread server.c room.o -o server