#include "server.h"
#include "pthread.h"
#include "room.h"

#define MAX_CLI_SEATS       99

typedef struct {
    // Access to room information
    Room *room;

    // Unitary buffer for requests (shared between server and threads)
    char *buffer;

    // Mutex and condition variable to keep buffer access synced (threads and server)
    pthread_mutex_t *mut_requestBuffer;
    pthread_cond_t *cvar_requestBuffer;
} officeTicketInfo;

typedef struct {
    int clientID;
    unsigned int numPlaces;
    int* placePreferences;
} Request;

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

/**
 * @brief Checks if a seat is free
 *
 * @param seats
 * @param seatNum
 * @retval 0 seat already booked
 * @retval 1 seat free
 */
int isSeatFree(Seat *seats, int seatNum);

/**
 * @brief Updates the seat state
 *
 * @see isSeatFree this function doesn't do any kind of verification)
 *
 * @param seats A pointer to seats array
 * @param seatNum The seat number (identifier)
 * @param clientID The client's ID that books this seat
 */
void bookSeat(Seat *seats, int seatNum, int clientID);

/**
 * @brief
 *
 * @param seats
 * @param seatNum
 */
void freeSeat(Seat *seats, int seatNum);

void answerClient();

void logSuccessRequest();

void logFailedSuccess();
