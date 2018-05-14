flags = -Wall -Wextra -g
thread_flags = -lpthread -pthread
all: clean client server
# client section
client: log.o
	gcc $(flags) client.c log.o -o client

# server section
server: room.o officeTicket.o log.o
	gcc $(flags) $(thread_flags) server.c room.o log.o officeTicket.o -o server

room.o: 
	gcc $(flags) -c room.c -o room.o

officeTicket.o: room.o
	gcc $(flags) -c officeTicket.c -o officeTicket.o
# shared libs
log.o:
	gcc $(flags) -c log.c

# clean
clean:
	rm -f server
	rm -f client
	rm -f *.o