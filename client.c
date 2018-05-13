#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "client.h"
#include "log.h"

static int timeout = 0; // flag

int main(int argc, char *argv[])
{
	printf("** Running process %d (PGID %d) **\n", getpid(), getpgrp());

	if (argc == 4)
		printf("ARGS: %s | %s | %s\n", argv[1], argv[2], argv[3]);
	else {
		printf("Invalid arguments\n");
		printf("Usage : %s <time_out> <num_wanted_seats> <pref_seat_list>\n", argv[0]);
		exit(ERROR_INVALID_ARGUMENTS);
	}

	// TODO why is this here?	
	sleep(1);

	// create FIFO ans<pid>
	char* fifoName = createFIFO();

	// register handler for SIG_ALARM
	int timeout_arg = atoi(argv[1]);
	if(signal(SIGALRM, timeoutHandler) == SIG_ERR) {
		perror(NULL);
		exit(SIGALRM_ERROR);
	}
	
	// send request to server
	int num_wanted_seats = atoi(argv[2]);
	sendRequest(num_wanted_seats, argv[3]);

	// set alarm
	alarm(timeout_arg);

	// attempt to get server answer and log it
	getServerAnswer(fifoName);

	// release allocated memory for fifoName
	free(fifoName);

	return 0;
}

char* createFIFO()
{	
	// get the process id
	pid_t pid = getpid();

	// by default, pids are integers, which max length is 5
	// (but that is configurable)
	// the extra 4 bytes are for '\0' and "ans" at the beggining
	char *pathName = (char*)malloc(PID_LENGTH+4);
	if(pathName == NULL) {
		fprintf(stderr, "Couldn't allocate memory\n");
		exit(FIFO_CREATION_ERROR);
	}

	// build the pathName
	sprintf(pathName, "ans%d", pid);

	// create FIFO
	if(mkfifo(pathName, 0666)) {
		free(pathName);
		perror(NULL);
		exit(FIFO_CREATION_ERROR);
	}

	return pathName;
}

void sendRequest(int num_wanted_seats, char* pref_seat_list) {
	// get the pid
	pid_t pid = getpid();

	// parse pref_seat_list
	int capacity = num_wanted_seats; // initial memory capacity
	int i = 0; // counter
	int* pref_list = malloc(capacity*sizeof(int));
	char* temp = strtok(pref_seat_list, " ");
	while(temp != NULL) {
		// double the allocated memory if needed
		if((i + 1) == capacity) {
			capacity *= 2;
			pref_list = realloc(pref_list, capacity*sizeof(int));
		}

		// add number to list
		pref_list[i++] = atoi(temp);

		// search next token
		temp = strtok(NULL, " ");
	}


	// open the FIFO requests
	printf("client %d is opening request FIFO\n", pid);
	int fd;
	if((fd = open(SERVER_REQUEST_FIFO, O_WRONLY)) == -1) {
		perror(NULL);
		exit(OPEN_SERVER_FIFO_ERROR);
	}

	// write first packet <lenght pref list> <client id> <num wanted seats>
	printf("client %d is writing request on FIFO\n", pid);
	int packet[] = {i, pid, num_wanted_seats}; 
	if(write(fd, packet, 3*sizeof(int)) == -1) {
		perror(NULL);
		exit(WRITE_SERVER_FIFO_ERROR);
	}

	// write the list of preferences
	if(write(fd, pref_list, i*sizeof(int)) == -1) {
		perror(NULL);
		exit(WRITE_SERVER_FIFO_ERROR);
	}

	// release resources
	printf("client %d sent request\n", pid);
	close(fd);
	free(pref_list);
}

void timeoutHandler(int signal) {
	// update flag
	timeout = 1;
	printf("timeout!\n");
}

void getServerAnswer(char* fifoName) {
	// open the fifo
	printf("Let's find the server answer! Opening fifo %s\n", fifoName);
	int fd;
	if((fd = open(fifoName, O_RDONLY | O_NONBLOCK)) == -1) {
		// free heap memory
		free(fifoName);
		perror(NULL);
		exit(ERROR_OPEN_ANSWER_FIFO);
	}
	while(!timeout) {
		// attemp to read the first 4 bytes, which indicate error or success (number of booked seats actually)
		int num_booked_seats;
		ssize_t count;
		if((count = read(fd, &num_booked_seats, sizeof(int))) == -1) {
			if(errno != EAGAIN) {
				perror(NULL);
				exit(1);
			} else {
				continue;
			}
		} else if(count == 0) {
			continue;
		}

		// the first number indicates whenever the request was successful or not
		printf("%d booked seats!\n", num_booked_seats);
		if(num_booked_seats < 0) {
			clientLogBookFailed(getpid(), num_booked_seats);
			break;
		}
		else {
			// read the list of booked seats
			int* list_booked_seats = (int*) malloc(num_booked_seats*sizeof(int));
			read(fd, list_booked_seats, num_booked_seats*sizeof(int));

			// log
			clientLogBookSuccess(getpid(), num_booked_seats, list_booked_seats);

			// release memory
			free(list_booked_seats);

			break;
		}
	}

	if(timeout) {
		printf("client tired of waiting\n");
		clientLogBookFailed(getpid(), -7);
	}

	// close fifo
	close(fd); 

}