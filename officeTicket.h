#include <pthread.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "room.h"

#define MAX_CLI_SEATS 99

/*
union semun
{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
}*/

typedef struct
{
	int clientID;
	unsigned int numSeats;
	unsigned int numSeatsPreferences;
	int * seatsPreferences;
	int isTaken;
} Request;

typedef struct
{
	// Access to room information
	Room room;

	// Unitary buffer for requests (shared between server and threads)
	Request *request;

	// Mutex and condition variable to keep buffer access synced (threads and server)
	pthread_mutex_t *mut_requestBuffer;
	pthread_cond_t *cvar_requestBufferFull;
	pthread_cond_t *cvar_requestBufferEmpty;

	// flag used by server to tell office tickets they must close
	int *isTimeOut;
} officeTicketInfo;

//pthread_mutex_t mut_synch = PTHREAD_MUTEX_INITIALIZER;
//static int officeTicketID;

/**
 * @brief Function that represents a thread (active office ticket)
 *
 * @param info A structure officeTicketInfo shared among threads and server
 * @return NULL
 */
void *enableOfficeTicket(void *info);


/**
 * @brief Determines if a request is valid, before attempting to process the request
 *
 * @retval 1 Valid request
 * @retval -1 The number of requested seats exceeds limit
 * @retval -2 The number of prefered seats is invalid
 * @retval -3 The seats requested are invalid (invalid identifiers)
 * @retval -4 Other errors
 */
int isValidRequest(Request *request, Room* room);

void answerClient();
