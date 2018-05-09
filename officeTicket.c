#include "officeTicket.h"

void* enableOfficeTicket(void* info){}

int isValidRequest(Request *request, Room *room){
    if(request->numPlaces > MAX_CLI_SEATS){
        return -1;  // Numero de lugares invalido
    }
    if(sizeof(request->placePreferences)/sizeof(request->placePreferences[0]) > request->numPlaces){
        return -2;  // Numero de identificadores invalido
    }
    for(unsigned int i = 0; i < sizeof(request->placePreferences)/sizeof(request->placePreferences[0]); i++){
        if(request->placePreferences[i] < 0 ||
           request->placePreferences[i] > room->numberSeats){
               return -3;   // identificadores invalidos
           }
    }

    int isOneFree = 0;
    for(unsigned int i = 0; i < sizeof(room->seats)/sizeof(room->seats[0]); i++){
        if(seats[i].status == FREE){
            isOneFree = 1;
        }
    }
    if(!isOneFree){
        return -6;  // Sala cheia
    }

    for(unsigned int i = 0; i < sizeof(request->placePreferences)/sizeof(request->placePreferences[0]); i++){
        if(room->seats[request->placePreferences[i]].status == BOOKED){
               return -5;   // Pelo menos um dos lugares pedidos está ocupado
           }
    }

    if(request->numPlaces < 0){
        return -4; // Numero de lugares invalido (outros erros em parametros)
    }

    return 1;
}

int isSeatFree(Seat *seats, int seatNum){}

void bookSeat(Seat *seats, int seatNum, int clientID){}

void freeSeat(Seat *seats, int seatNum){}

void answerClient(){}

void logSuccessRequest();

void logFailedSuccess();
