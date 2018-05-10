#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "client.h"

int main(int argc, char *argv[])
{
	printf("** Running process %d (PGID %d) **\n", getpid(), getpgrp());

	if (argc == 4)
		printf("ARGS: %s | %s | %s\n", argv[1], argv[2], argv[3]);

	sleep(1);

	return 0;
}

void createFIFO()
{	
	// get the process id
	pid_t pid = getpid();

	// by default, pids are integers, which max length is 5
	// but that is configurable, let's assume a max length of 10, a long length (PID_LENGTH)
	// the extra 4 bytes are for '\0' and "ans" at the beggining
	char pathName[PID_LENGTH + 4];
	sprintf(pathName, "ans%d", pid);

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