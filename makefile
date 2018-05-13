all: clean client server
# client section
client: log.o
	gcc -Wall -Wextra -g client.c log.o -o client

# server section
server: room.o officeTicket.o
	gcc -Wall -Wextra -lpthread -lrt -pthread server.c room.o officeTicket.o -o server

room.o: 
	gcc -Wall -Wextra -c room.c -o room.o

officeTicket.o: room.o
	gcc -Wall -Wextra -c officeTicket.c -o officeTicket.o
# shared libs
log.o:
	gcc -Wall -Wextra -g -c log.c

# clean
clean:
	rm -f server
	rm -f client
	rm -f *.o