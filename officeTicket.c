#include "officeTicket.h"

void* enableOfficeTicket(void* info)
{

  officeTicketInfo* infoTicket = (officeTicketInfo*) info;
  Request req;
  char* infoBuf;
  void * retValue = malloc(sizeof(int));
  int validRequest = 0; //request success flag
  int numSeatsToBook = req.numSeatsPreferences;
  sem_t* semArray[numSeatsToBook];
  int bookedSeats[numSeatsToBook] = { 0 };
  int numBookedSeats = 0;

  pthread_mutex_lock(&mut_synch);
  if(infoTicket->buffer != NULL){
    infoBuf = infoTicket->buffer;
    infoTicket->buffer = NULL;
  }
  pthread_mutex_unlock(&mut_synch);

  req = parseRequest(infoBuf);



  for(int i = 0; i < numSeatsToBook; i++) //Criação do grupo de semaforos
  {
    char str[12];
    sprintf(str, "%d", req.seatsPreferences[i]);

    //Se calhar retirar o O_EXCL fazia com que o if seguinte deixasse de ser necessário.
    semArray[i] = sem_open(str, O_CREAT || O_EXCL, S_IRUSR || S_IWUSR, 0);

    if(semArray[i] == SEM_FAILED)
    semArray[i] = sem_open(str, O_RDWR);
  }


  if(infoTicket->room.numberSeats == infoTicket->room.occupiedSeats) //the room is full
  {
    *((int*) retValue) = 6;
    return retValue;
  }

  for(int i = 0; i < numSeatsToBook; i++) //lock dos semaforos
    sem_wait(semArray[i]);

  for(int i = 0; i < numSeatsToBook; i++)
  {
    if(infoTicket->room.seats[req.seatsPreferences[i]].status == FREE)
    {
      infoTicket->room.seats[req.seatsPreferences[i]].status = BOOKED;
      infoTicket->room.occupiedSeats++;
      bookedSeats[numBookedSeats] = req.seatsPreferences[i];;
      numBookedSeats++;
    }
    if(numBookedSeats == req.numSeats)
    {
      validRequest = 1;
      break;
    }
  }

  if(!validRequest) //the request was not completed
  {
    for(int h = 0; h < bookedSeats; h++) //free booked seats
      infoTicket->room.seats[freeSeats[h]].status = FREE;

    for(int j = 0; j < numSeats; j++) //unlock dos semaforos
      sem_post(semArray[j]);

    *((int*) retValue) = 5;
    return retValue;
  }
  else //the request was successful
  {
    for(int j = 0; j < numSeats; j++) //unlock dos semaforos
      sem_post(semArray[j]);

      
  }
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
  req.numSeats = num;

  while(temp != NULL)
  {
    temp = strtok(NULL, " \n");
    num = atoi(temp);
    req.seatsPreferences[count] = num;
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
  if (request->numSeatsPreferences < request->numSeats || request->numSeatsPreferences > MAX_CLI_SEATS)
  return -2;

  // check if requested seats exist
  for (int i = 0; i < request->numSeatsPreferences; i++)
  if (request->seatsPreferences[i] < 1 || request->seatsPreferences[i] > room->numberSeats)
  return -3;

  if (request->numSeats < 0)
  {
    return -4; // Numero de lugares invalido (outros erros em parametros)
  }

  return 1;
}

void answerClient(){};
