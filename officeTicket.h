#include "server.h"
#include "pthread.h"
#include "room.h"

#define MAX_CLI_SEATS       99

typedef struct {
    // Access to room information
    Room room;

    // Unitary buffer for requests (shared between server and threads)
    char *buffer;

    // Mutex and condition variable to keep buffer access synced (threads and server)
    pthread_mutex_t *mut_requestBuffer;
    pthread_cond_t *cvar_requestBufferFull;
	pthread_cond_t *cvar_requestBufferEmpty;
} officeTicketInfo;

typedef struct {
    int clientID;
    unsigned int numPlaces;
    int* placePreferences;
} Request;

static int officeTicketID;

/**
 * @brief Function that represents a thread (active office ticket)
 *
 * @param info A structure officeTicketInfo shared among threads and server
 * @return NULL
 */
void* enableOfficeTicket(void* info);

/**
 * @brief Determines if a request is valid
 *
 * @return int
 */
int isValidRequest(Request *request, int numSeats);

void answerClient();