all: client

client: 
	gcc -Wall -Wextra client.c -o client

clean:
	rm -f client
	rm -f *.o
	