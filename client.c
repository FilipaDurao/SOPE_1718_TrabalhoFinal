#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
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

int createFIFO()
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