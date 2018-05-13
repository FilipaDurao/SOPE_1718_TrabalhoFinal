#include "room.h"
#include <stdlib.h>

Seat createSeat(int number) {
    Seat s;
    s.number = number;
    s.status = FREE;
    s.sem_unlocked = malloc(sizeof(sem_t));
    sem_init(s.sem_unlocked, 0, 1);
    return s;
}

Room createRoom(int numberSeats) {
    Room r = {numberSeats, NULL};
    r.seats = (Seat*) malloc(sizeof(Seat)*numberSeats);

    for(int i = 0; i < numberSeats; i++) 
        r.seats[i] = createSeat(i+1);

    return r;  
}

void deleteRoom(Room r) {
    // destroy semaphores
    for(int i = 0; i < r.numberSeats; i++) {
        sem_destroy(r.seats[i].sem_unlocked);
        free(r.seats[i].sem_unlocked);
    }
    // free array of seats
    if(r.seats != NULL)
        free(r.seats);
}

int isSeatFree(Seat *seats, int seatNum){
    return seats[seatNum - 1].status;
}

void bookSeat(Seat *seats, int seatNum, int clientId){
    seats[seatNum - 1].status = BOOKED;
    seats[seatNum - 1].clientID = clientId;
}

void freeSeat(Seat *seats, int seatNum){
    seats[seatNum - 1].status = FREE;
}