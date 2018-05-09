#include "server.h"
#include "pthread.h"
#include "room.h"

#define MAX_CLI_SEATS 99

typedef struct
{
	// Access to room information
	Room room;

	// Unitary buffer for requests (shared between server and threads)
	char *buffer;

	// Mutex and condition variable to keep buffer access synced (threads and server)
	pthread_mutex_t *mut_requestBuffer;
	pthread_cond_t *cvar_requestBufferFull;
	pthread_cond_t *cvar_requestBufferEmpty;
} officeTicketInfo;

typedef struct
{
	int clientID;
	unsigned int numSeats;
	unsigned int numSeatsPreferences;
	int *seatsPreferences;
} Request;

static int officeTicketID;

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
int isValidRequest(Request *request, int numSeats);

void answerClient();