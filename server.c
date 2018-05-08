#include <sys/types.h>
#include <sys/stat.h>
#include "pthread.h"
#include <fcntl.h>

#include "server.h"
#include "officeTicket.h"

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
    char requestBuffer[200] = NULL; // TODO adjust buffer size

    // Mutex and condition variable (server <-> threads)
    pthread_mutex_t mut_requestBuffer;
    pthread_cond_t cvar_requestBuffer;
    
    // pack the information to be sent to all threads
    officeTicketInfo ot_info = {
        s.room,
        &requestBuffer,
        &mut_requestBuffer,
        &cvar_requestBuffer
    };

    // create threads
    pthread_t *officeTickets = (pthread_t*)malloc(sizeof(pthread_t)*s.numOfficeTickets);
    for(int i = 0; i < s.numOfficeTickets; i++) {
        pthread_create(officeTickets + i, NULL, enableOfficeTciket, (void*) &ot_info);
    }

    /**
     * Server...
     */
    int fd = open("requests", O_NONBLOCK | O_RDONLY);

    // the loop ends upon timeout, need to update this
    while(1) {
        pthread_mutex_lock(&mut_requestBuffer);

        while(requestBuffer != NULL) // there still is a pendent request
            pthread_cond_wait(&cvar_requestBuffer, &mut_requestBuffer);

        // fetch some request from the FIFO
        ssize_t rbytes;
        if(rbytes = read(fd, NULL, sizeof(NULL)) == 0) { // TODO define the request struct
             // nothing to read skipp
        } else if (rbytes > 0) {
            // fill buffer
        }

        // unlocks the buffer mutex and sends a signal to all office tickets
        pthread_cond_signal(&cvar_requestBuffer);
        pthread_mutex_unlock(&mut_requestBuffer);
    }

    // wait for threads to exit

}

static void createFIFO() {
    if(mkfifo("requests", 0660) != 0){
        // TODO
    }
}

static void closeFIFO() {

}