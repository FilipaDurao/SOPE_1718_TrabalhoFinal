#include <semaphore.h>
#include "room.h"

typedef struct
{
	int clientID; // the client ID that requested this request
	unsigned int numSeats; // number of seats the client wants
	unsigned int numSeatsPreferences; // the lenght of seats preferences
	int * seatsPreferences; // the list of seats preferences
} Request;

typedef struct
{
	Room room; // the room
	Request *request; // Unitary buffer for requests (shared between server and threads)
	sem_t *sem_buffer_empty; // semaphore to sync access to requests buffer
	sem_t *sem_buffer_full; // semaphore to sync access to requests buffer
	sem_t *sem_access; // semaphore to sync threads when blocking seats access
	int *isTimeOut; // a flag used by server to tell threads they should close/end
} officeTicketInfo;


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


/**
 * @brief Processes the request, booking seats incrementally
 * 
 * @param info 
 * @retval int* List of allocated seats (dynamically allocated)
 * @retval NULL The request failed, no seats were booked
 */
int* processRequest(officeTicketInfo *info);

/**
 * @brief 
 * needs client id
 * number of booked seats
 * list of booked seats
 * @param req 
 */
void answerClient(int threadID, Request *req, int *list_booked_seats, int errorCode);