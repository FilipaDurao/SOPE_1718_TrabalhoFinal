all: client

client: 
	gcc client.c -o client

clean:
	rm -f client
	rm -f *.o
	