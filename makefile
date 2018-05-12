all: server

server: room.o
	gcc -Wall -Wextra -lpthread server.c room.o -o server

room.o:
	gcc -Wall -Wextra -c room.c -o room.o