#include "officeTicket.h"
#include <stdio.h>

#include <unistd.h>
static int officeTicketID = 0;
void* enableOfficeTicket(void* info){
	int myThreadID = ++officeTicketID;
	printf("Hello from thread %d\n", myThreadID);
	officeTicketInfo* infoTicket = (officeTicketInfo*) info;
  officeTicketInfo* infoTicket = (officeTicketInfo*) info;
  Request req;
  void * retValue = malloc(sizeof(int));
  char* infoBuf;
  int validRequest = 0; //request success flag
  int numSeatsToBook = req.numSeatsPreferences;
  sem_t* semArray[numSeatsToBook];
  int bookedSeats[numSeatsToBook] = { 0 };
  int numBookedSeats = 0;

	while(!*(infoTicket->isTimeOut)) {
		pthread_mutex_lock(infoTicket->mut_requestBuffer);
		while(infoTicket->request->isTaken) {
			// wait for a new request
			printf("Thread %d: No requests!\n", myThreadID);
			pthread_cond_wait(infoTicket->cvar_requestBufferFull, 
				infoTicket->mut_requestBuffer);
		}

		// yay new request, copy it and unlock the mutex
		Request myRequest = *(infoTicket->request);
		infoTicket->request->isTaken = 1;
		pthread_cond_signal(infoTicket->cvar_requestBufferEmpty);
		pthread_mutex_unlock(infoTicket->mut_requestBuffer);
		printf("Thread %d, Received request from client %d\n", myThreadID,myRequest.clientID);

  int numSeats = req.numSeats;
  sem_t* semArray[numSeats];

	printf("Thread %d realised that it's time to close doors!\n", myThreadID);
//   Request req;
//   char* infoBuf;

  for(int i = 0; i < numSeatsToBook; i++) //Criação do grupo de semaforos
  {
    sprintf(str, "%d", req.seatsPreferences[i]);
    char str[12];


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

//     if(semArray[i] == SEM_FAILED)
//     semArray[i] = sem_open(str, NULL);
//   }

//   int flag = 0;
//   while(!flag) //TODO: O que está dentro deste while não está correto ou acabado
//   {
//     flag = 1;

//     for(int i = 0; i < numSeats; i++) //lock dos semaforos
//       if(sem_trywait(semArray[i]) == -1)   //TODO falta fazer uma condiçao para o caso de um dos seat estar a ser acedido por outro thread
//         flag = 0;

//     for(int i = 0; i < numSeats; i++)
//     {
//       if(infoTicket.Room.seats[req.seatsPreferences[i]]->seatStatus == FREE)
//         infoTicket.Room.seats[req.seatsPreferences[i]]->seatStatus = BOOKED;
//       else
//       {
//         flag = 1;
//         break;
//       }
//     }
//     for(int j = 0; j < numSeats; j++) //unlock dos semaforos
//       sem_post(semArray[i]);
//   }
	return NULL;
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
	for (unsigned int i = 0; i < request->numSeatsPreferences; i++)
		if (request->seatsPreferences[i] < 1 || request->seatsPreferences[i] > room->numberSeats)
			return -3;

	if (request->numSeats < 0) {
		return -4; // Numero de lugares invalido (outros erros em parametros)
	}

	return 1;
}

void answerClient(){};
