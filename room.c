#include "room.h"
#include <stdlib.h>

Seat createSeat(int number) {
    Seat s = {number, -1, FREE};
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