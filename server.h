#include "officeTicket.h"

#define FIFO_NAME "requests"
typedef struct {
    int numOfficeTickets;
    int officeTicketsDuration;
    char* requestBuffer;
    Room room;
} Server;

/**
 * @brief Create a Server object and initializes the room filling it with free seats
 * 
 * @param numSeats The event's room capacity
 * @param numOfficeTickets The number of office tickets to serve costumers
 * @param officeTicketsDuration For how long, in seconds, the office tickets are open
 * @return Server 
 */
Server createServer(int numSeats, int numOfficeTickets, int officeTicketsDuration);

/**
 * @brief This is function is called when the server should be enabled.
 * Creates the FIFO requests
 * Opens the office tickets 
 * It will loop until the office tickets close, 
 *  loading requests from the clients to the unitary buffer which office tickets can access
 * When office tickets close, the server tells them to do so, and closes the FIFO
 */
void enableServer(Server s);

/**
 * @brief 
 * 
 */
void disableServer();

/**
 * @brief Get the Request object
 * 
 * @param request 
 * @return int 
 */
int getRequest(int fd, Request *request);


void closeOfficeTicketsHandler(int signal);