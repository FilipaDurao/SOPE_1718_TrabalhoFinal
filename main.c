#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>

#include "constants.h"
#include "Bilheteira.h"

int isSeatFree(Seat *seats, int seatNum);
void bookSeat(Seat *seats, int seatNum, int clientId);
void freeSeat(Seat *seats, int seatNum);

int main(int argc, char** argv){

    int numRoomSeats = argv[1];
    int numTicketOffices = argv[2];
    int timeOut = argv[3];
    int requests;

    Seat* room = malloc(numRoomSeats*sizeof(Seat));

    if(mkfifo("requests", 0660) =! 0){
        //TODO erro cenas
    }

    requests = open("requests", O_RDONLY);

    char* request;  // Buffer Unitario que guarda o proximo request a ser atendido
    pthread_t* threadIds = malloc(numTicketOffices*sizeof(pthread_t));

    for(unsigned int i = 0; i <= numTicketOffices; i++){
        pthread_create(threadIds[i], NULL, ticketOffice, (void*)request);
    }



}

void* ticketOffice(void* arg){
    //TODO
}
