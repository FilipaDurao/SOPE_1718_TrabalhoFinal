#include "officeTicket.h"

void* enableOfficeTicket(void* info){
  officeTicketInfo* infoTicket = (officeTicketInfo*) info;
  Request req;
  char* infoBuf;

  pthread_mutex_lock(&mut_synch);
  if(infoTicket->buffer != NULL){
    infoBuf = infoTicket->buffer;
    infoTicket->buffer = NULL;
  }
  pthread_mutex_unlock(&mut_synch);

  req = parseRequest(infoBuf);

}

Request parseRequest(char* requestString)
{
  Request req;
  int num;
  int count = 0;
  char* temp;

  temp = strtok(requestString, " \n");
  num = atoi(temp);
  req.clientID = num;

  temp = strtok(NULL, " \n");
  num = atoi(temp);
  req.numPlaces = num;

  while(temp != NULL)
  {
    temp = strtok(NULL, " \n");
    num = atoi(temp);
    req.placePreferences[count] = num;
    count++;
  }
  return req;
}


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
        if(room->seats[i].status == FREE){
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


void answerClient(){}

void logSuccessRequest();

void logFailedSuccess();
