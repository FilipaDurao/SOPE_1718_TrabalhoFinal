#include "room.h"

typedef struct {
    int numOfficeTickets;
    char* requestBuffer;
    Room room;
} Server;

/**
 * @brief Create a Server object
 * 
 * @param numSeats The event's room capacity
 * @param numOfficeTickets The number of office tickets to serve costumers
 * @param officeTicketsTime For how long, in seconds, the office tickets are open
 * @return Server 
 */
Server createServer(int numSeats, int numOfficeTickets, int officeTicketsTime);

/**
 * @brief Deletes a server, releasing dynamically allocated memory
 * 
 * @param s 
 */
void deleteServer(Server s);