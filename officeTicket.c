#include "officeTicket.h"
#include <stdio.h>

#include <unistd.h>

static int officeTicketID = 0;

void* enableOfficeTicket(void* info){
	int myThreadID = ++officeTicketID;
	printf("Hello from thread %d\n", myThreadID);
	officeTicketInfo* infoTicket = (officeTicketInfo*) info;

	while(!infoTicket->isTimeOut) {
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

		// process the request
		//free(myRequest.seatsPreferences);
	}
//   Request req;
//   char* infoBuf;

//   pthread_mutex_lock(&mut_synch);
//   if(infoTicket->buffer != NULL){
//     infoBuf = infoTicket->buffer;
//     infoTicket->buffer = NULL;
//   }
//   pthread_mutex_unlock(&mut_synch);

//   req = parseRequest(infoBuf);

//   int numSeats = req.numSeats;
//   sem_t* semArray[numSeats];


//   //semget(IPC_PRIVATE, numSeats, IPC_CREAT);


//   for(int i = 0; i < numSeats; i++) //Criação do grupo de semaforos
//   {
//     char str[12];
//     sprintf(str, "%d", req.seatsPreferences[i]);

//     //Se calhar retirar o O_EXCL fazia com que o if seguinte deixasse de ser necessário.
//     semArray[i] = sem_open(str, O_CREAT || O_EXCL, S_IRUSR || S_IWUSR, 0);

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
