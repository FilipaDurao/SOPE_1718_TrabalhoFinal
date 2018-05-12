#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "server.h"

static void createFIFO() {
    if(mkfifo("requests", 0660) != 0){
        // TODO
    }
}

int main(int argc, char** argv){

    if(argc != 4) {
        exit(1);
    }

    int numRoomSeats = atoi(argv[1]);
    int numTicketOffices = atoi(argv[2]);
    int openTime = atoi(argv[3]);
    Server s = createServer(numRoomSeats, numTicketOffices, openTime);
    enableServer(s);
    
}

Server createServer(int numSeats, int numOfficeTickets, int officeTicketsDuration) {
    // build the struct
    Server s;
    s.numOfficeTickets = numOfficeTickets;
    s.officeTicketsDuration = officeTicketsDuration;
    
    // build the room
    s.room = createRoom(numSeats);

    return s;
}

void enableServer(Server s) {
    /**
     * Create the fifo requests
     */
    createFIFO();

    /**
     * Launch threads
     */
    // buffer declaration
    Request requestBuffer;
    requestBuffer.isTaken = 1;

    // Mutex and condition variable (server <-> threads)

    // The mutex for sync the buffer access
    pthread_mutex_t mut_requestBuffer = PTHREAD_MUTEX_INITIALIZER;
    
    // cvar_requestBufferFull is used by server to tell threads that a new request is available
    pthread_cond_t cvar_requestBufferFull = PTHREAD_COND_INITIALIZER;
    
    // cvar_requestBufferEmpty is used by threads to tell server the previous request is being handled, therefore the buffer is empty
    pthread_cond_t cvar_requestBufferEmpty = PTHREAD_COND_INITIALIZER;
    
    // pack the information to be sent to all threads
    officeTicketInfo ot_info = {
        s.room,
        &requestBuffer,
        &mut_requestBuffer,
        &cvar_requestBufferFull,
        &cvar_requestBufferEmpty
    };

    // create threads
    //pthread_mutex_lock(&mut_requestBuffer);
    pthread_t *officeTickets = (pthread_t*)malloc(sizeof(pthread_t)*s.numOfficeTickets);
    for(int i = 0; i < s.numOfficeTickets; i++) {
        pthread_create(officeTickets + i, NULL, enableOfficeTicket, (void*) &ot_info);
    }
    /**
     * Server...
     */
    int fd = open("requests", O_RDONLY);
    printf("server opened requests FIFO\n");

    // the loop ends upon timeout, need to update this
    while(1) {

        pthread_mutex_lock(&mut_requestBuffer);
        while(!requestBuffer.isTaken) // there still is a pendent request
            pthread_cond_wait(&cvar_requestBufferEmpty, &mut_requestBuffer);

        // try to fetch some request from the FIFO
        if(getRequest(fd, &requestBuffer) == 0) {
            // new request
            // unlocks the buffer mutex and sends a signal to all office tickets
            pthread_cond_signal(&cvar_requestBufferFull);
            pthread_mutex_unlock(&mut_requestBuffer);
        } else {
            pthread_mutex_unlock(&mut_requestBuffer);
        }        
    }

    // wait for threads to exit

    // kill condition variables and mutex
    pthread_cond_destroy(&cvar_requestBufferFull);
    pthread_cond_destroy(&cvar_requestBufferEmpty);
    pthread_mutex_destroy(&mut_requestBuffer);

}

int getRequest(int fd, Request *request) {
    int packet[3];
    ssize_t read_size;

    // read the first three numbers
    // <lenght pref list> <client id> <num wanted seats>
	if((read_size = read(fd, packet, sizeof(int)*3)) == 0)
        return -1; // nothing to read

    request->numSeatsPreferences = packet[0];
    request->clientID = packet[1];
    request->numSeats = packet[2];

	//printf("%d %d %d\n", packet[0], packet[1], packet[2]);

    // read the list of preferences
	request->seatsPreferences = malloc(sizeof(int)*packet[0]);
	read(fd, request->seatsPreferences, sizeof(int)*packet[0]);

    // set flag
    request->isTaken = 0;
    printf("server found new request\n");
    return 0;
}
