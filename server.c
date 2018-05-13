#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include "server.h"

int isTimeOut = 0;

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
    return 0;
}

Server createServer(int numSeats, int numOfficeTickets, int officeTicketsDuration) {
    // build the struct
    Server s;
    s.numOfficeTickets = numOfficeTickets;
    s.officeTicketsDuration = officeTicketsDuration;
    
    // build the room
    s.room = createRoom(numSeats);

    // install alarm handler
    signal(SIGALRM, closeOfficeTicketsHandler);

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
    // they need access to room, buffer, timeOut flag and mutex and condition variables
    officeTicketInfo ot_info = {
        s.room,
        &requestBuffer,
        &mut_requestBuffer,
        &cvar_requestBufferFull,
        &cvar_requestBufferEmpty, 
        &isTimeOut
    };

    /**
     * create threads
     * we first block the SIGALARM signal on this main thread.
     * the created threads will inherit this signal mask
     * after creating the threads, we unblock the sigalarm only on the main thread
     * only the main thread must handle SIGALARM
     * 
     */
    // block SIGALARM
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGALRM);
    if(pthread_sigmask(SIG_BLOCK, &sigset, NULL) != 0) {
        perror("Couldn't block SIGALARM");
        exit(1);
        // TODO release other resources too, maybe create a dedicated function
    }

    // create threads
    pthread_t *officeTickets = (pthread_t*)malloc(sizeof(pthread_t)*s.numOfficeTickets);
    for(int i = 0; i < s.numOfficeTickets; i++) {
        pthread_create(officeTickets + i, NULL, enableOfficeTicket, (void*) &ot_info);
    }

    // unblock SIGALARM only on this main thread
    if(pthread_sigmask(SIG_UNBLOCK, &sigset, NULL) != 0) {
        perror("Couldn't unblock SIGALARM");
        exit(1);
        // TODO release other resources too, maybe create a dedicated function
        // TODO release officeTickets too
    }

    // set alarm
    alarm(s.officeTicketsDuration);

    /**
     * Server...
     */
    int fd = open("requests", O_RDONLY);
    printf("server opened requests FIFO\n");

    // the loop ends upon timeout
    while(!isTimeOut) {

        pthread_mutex_lock(&mut_requestBuffer);
        printf("server: locked mutex\n");
        while(!requestBuffer.isTaken) { // there still is a pendent request
            printf("server: there's a pendent request, I will wait..\n");
            pthread_cond_wait(&cvar_requestBufferEmpty, &mut_requestBuffer);
        }

        // try to fetch some request from the FIFO
        printf("server: I will try to fetch a new request from the FIFO\n");
        if(getRequest(fd, &requestBuffer) == 0) {
            printf("server: new request found!\n");
            // new request
            // unlocks the buffer mutex and sends a signal to all office tickets
            pthread_cond_signal(&cvar_requestBufferFull);
            pthread_mutex_unlock(&mut_requestBuffer);
        } else {
            printf("server: request not found!\n");
            pthread_mutex_unlock(&mut_requestBuffer);
        }        
    }

    printf("server: So timeout... let's wait for threads!\n");
    // wait for threads to exit
    for(int i = 0; i < s.numOfficeTickets; i++) {
        pthread_join(officeTickets[i], NULL);
    }
    printf("HELLO FROM THE OTHER SIDE!\n");

    // kill condition variables and mutex
    pthread_cond_destroy(&cvar_requestBufferFull);
    pthread_cond_destroy(&cvar_requestBufferEmpty);
    pthread_mutex_destroy(&mut_requestBuffer);

    free(officeTickets);
    // TODO release other resources too
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


void closeOfficeTicketsHandler(int signal) {
    printf("TIMEOUT!\n");
    isTimeOut = 1;
}