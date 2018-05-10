#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "client.h"

static int timeout = 0; // flag

int main(int argc, char *argv[])
{
	printf("** Running process %d (PGID %d) **\n", getpid(), getpgrp());

	if (argc == 4)
		printf("ARGS: %s | %s | %s\n", argv[1], argv[2], argv[3]);
	else {
		printf("Invalid arguments\n");
		printf("Usage : %s <time_out> <num_wanted_seats> <pref_seat_list>", argv[0], argv[1], argv[2], argv[3]);
		exit(ERROR_INVALID_ARGUMENTS);
	}

	// TODO why is this here?	
	sleep(1);

	// create FIFO ans<pid>
	char* fifoName = createFIFO();

	// register handler for SIG_ALARM
	int timeout = atoi(argv[1]);
	if(signal(SIGALRM, timeoutHandler) == SIG_ERR) {
		perror(NULL);
		exit(SIGALRM_ERROR);
		// TODO do I need to close FIFOs ?
	}

	// send request to server
	int num_wanted_seats = atoi(argv[2]);
	sendRequest(num_wanted_seats, argv[3]);
	
	// set alarm
	alarm(timeout);
	
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

	// build the request string
	char request[REQUEST_LENGTH];
	int request_size = sprintf(request, "%d %d %s*", pid, num_wanted_seats, pref_seat_list);

	// open the FIFO requests
	int fd;
	if((fd = open(SERVER_REQUEST_FIFO, O_WRONLY) == -1)) {
		perror(NULL);
		exit(OPEN_SERVER_FIFO_ERROR);
	}

	// write the request
	if(write(fd, request, request_size) == -1) {
		perror(NULL);
		exit(WRITE_SERVER_FIFO_ERROR);
	}
}

void timeoutHandler(int signal) {
	timeout = 1;
}

void getServerAnswer(char* fifoName) {
	// open the fifo

	int fd;
	if((fd = open(fifoName, O_RDONLY) == -1)) {
		// free heap memory
		free(fifoName);
		perror(NULL);
		exit(ERROR_OPEN_ANSWER_FIFO);
	}

	while(!timeout) {
		// attemp to read the first 4 bytes, which indicate error or success (number of booked seats actually)
		int num_booked_seats;
		read(fd, &num_booked_seats, sizeof(int));

		if(num_booked_seats < 0) {
			// call log function and pass error number
			break;
		}
		else {
			// read the list of booked seats
			int* list_booked_seats = (int*) malloc(num_booked_seats*sizeof(int));
			read(fd, list_booked_seats, num_booked_seats*sizeof(int));

			// log

			// release memory

			break;
		}
	}

	if(timeout) {
		// log timeout
	} 

}