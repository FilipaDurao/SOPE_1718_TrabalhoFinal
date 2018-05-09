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


int isValidRequest(Request *request, Room *room)
{
	// check if the amount of requested seats exceeds MAX_CLI_SEATS
	if (request->numSeats > MAX_CLI_SEATS)
		return -1;

	// check if the number of prefered seats is valid [number of wanted seats, MAX_CLI_SEATS]
	if (request->numSeatsPreferences < request->numSeats ||request->numSeatsPreferences > MAX_CLI_SEATS)
		return -2;

	// check if requested seats exist
	for (int i = 0; i < request->numSeatsPreferences; i++)
		if (request->seatsPreferences[i] < 1 || request->numSeatsPreferences[i] > room->numberSeats)
			return -3;

	if (request->numSeats < 0)
	{
		return -4; // Numero de lugares invalido (outros erros em parametros)
	}

	return 1;
}

void answerClient(){};