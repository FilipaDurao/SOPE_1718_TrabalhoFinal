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

	sleep(1);

	// create FIFO ans<pid>
	createFIFO();

	// send request to server
	int num_wanted_seats = atoi(argv[2]);
	sendRequest(num_wanted_seats, argv[3]);

	// register handler for SIG_ALARM and set alarm
	int timeout = atoi(argv[1]);
	if(signal(SIGALRM, timeoutHandler) == SIG_ERR) {
		perror(NULL);
		exit(SIGALRM_ERROR);
		// TODO do I need to close FIFOs ?
	}
	alarm(timeout);
	
	// loop and wait for server answer
	while(!timeout) {

	}

	// log
	
	return 0;
}

void createFIFO()
{	
	// get the process id
	pid_t pid = getpid();

	// by default, pids are integers, which max length is 5
	// (but that is configurable)
	// the extra 4 bytes are for '\0' and "ans" at the beggining
	char pathName[PID_LENGTH + 4];
	sprintf(pathName, "ans%ld", pid);

	// create FIFO
	if(mkfifo(pathName, 0666)) {
		perror(NULL);
		exit(FIFO_CREATION_ERROR);
	}
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