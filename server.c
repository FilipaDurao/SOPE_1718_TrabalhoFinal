#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <semaphore.h>
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

    // release resources
    deleteRoom(s.room);
    return 0;
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

    // buffer declaration, to be shared with threads
    Request requestBuffer;

    // the semaphores to sync buffer access
    sem_t sem_empty, sem_full, sem_access;       
    sem_init(&sem_empty, 0, 1); // todo check error codes
    sem_init(&sem_full, 0, 0); // todo check error codes
    sem_init(&sem_access, 0, 1); //
    // pack the information to be sent to all threads
    // they need access to room, buffer, timeOut flag and semaphores
    officeTicketInfo ot_info = {
        s.room,
        &requestBuffer,
        &sem_empty,
        &sem_full,
        &sem_access,
        &isTimeOut
    };

    /**
     * create threads
     * we first block the SIGALARM signal on this main thread.
     * the created threads will inherit this signal mask
     * after creating the threads, we unblock the sigalarm only on the main thread
     * only the main thread must handle SIGALRM
     * 
     */
    // block SIGALRM
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGALRM);
    if(pthread_sigmask(SIG_BLOCK, &sigset, NULL) != 0) {
        perror("Couldn't block SIGALRM");
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

    /**
     * Server...
     */
    int fd = open("requests", O_RDONLY);
    printf("server opened requests FIFO\n");

    
    // install alarm handler
    signal(SIGALRM, closeOfficeTicketsHandler);
    // set alarm
    printf("setting alarm for %d seconds. call returned: %d\n", s.officeTicketsDuration, alarm(s.officeTicketsDuration));

    // the loop ends upon timeout
    while(!isTimeOut) {

        // is the buffer empty ?
        if(sem_wait(&sem_empty)) {
            perror(NULL);
            exit(1);
        }
        printf("server: sem_empty = 0\n");
        
        // try to fetch some request from the FIFO
        printf("server: I will try to fetch a new request from the FIFO\n");
        int foundNewRequest;
        do {
            foundNewRequest = getRequest(fd, &requestBuffer);
        } while(foundNewRequest != 0 && !isTimeOut);

        // check why the loop ended
        if(isTimeOut) break;

        printf("server: new request found from client %d!\n", requestBuffer.clientID);
        if(sem_post(&sem_full)) {
            perror(NULL);
            exit(1);
        }
        printf("server: sem_full = 1\n");     
    }

    printf("server: So timeout... let's wait for threads!\n");
    // time is out
    // because threads are likely stucked at sem_wait(full), we will increment it to unlock all threads
    for(int i = 0; i < s.numOfficeTickets; i++) {
        sem_post(&sem_full);
        sem_post(&sem_access);
    } 
    // now that threads unlocked, they will detect the time is out and return. now we call pthread_join
    for(int i = 0; i < s.numOfficeTickets; i++) {
        pthread_join(officeTickets[i], NULL);
        printf("Thread %d returned!\n", i+1);
    }
    printf("HELLO FROM THE OTHER SIDE!\n");

    // destroy semaphores
    sem_destroy(&sem_empty);
    sem_destroy(&sem_full);
    sem_destroy(&sem_access);
    free(officeTickets);
    close(fd);
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

    return 0;
}


void closeOfficeTicketsHandler(int signal) {
    printf("TIMEOUT!\n");
    isTimeOut = 1;
}