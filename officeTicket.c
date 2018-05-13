#include "officeTicket.h"
#include <stdio.h>
#include <unistd.h>

static int officeTicketID = 1;

void* enableOfficeTicket(void* arg) {
	// set a thread ID
	int myOfficeTicketID = officeTicketID++;
	printf("Hello from thread %d\n", myOfficeTicketID);

	// cast the argument and separate some of content for easier access
	officeTicketInfo *ot_info = (officeTicketInfo *)arg;
	Request *req = ot_info->request;

	while(1) {
		// is there any pendent request ?
		if(sem_wait(ot_info->sem_buffer_full)) {
			perror(NULL);
		}
		printf("Thread %d got access to request buffer!\n", myOfficeTicketID);
		// the server may indicate there are pendent requests to unlock threads so that they can terminate
		if(*(ot_info->isTimeOut)) break;

		// copy the request from the buffer. releasing the buffer as fast as possible
		Request myRequest = *req;
		printf("Thread %d got the request from client %d\n", myOfficeTicketID, myRequest.clientID);
		// set buffer as empty
		if(sem_post(ot_info->sem_buffer_empty)) {
			perror(NULL);
		}

		// process the request
	}

	pthread_exit(NULL);
}
/*
void* enableOfficeTicket(void* info){
	int myThreadID = ++officeTicketID;
	printf("Hello from thread %d\n", myThreadID);

	// variable declaration
	officeTicketInfo *infoTicket = (officeTicketInfo *)info;
	Request *req = infoTicket->request;
	void *retValue = malloc(sizeof(int));
	char *infoBuf;
	int validRequest = 0; //request success flag
	int numSeatsToBook = req->numSeatsPreferences;

	int bookedSeats[numSeatsToBook] = {0};
	int numBookedSeats = 0;

	while (!*(infoTicket->isTimeOut))
	{
		pthread_mutex_lock(infoTicket->mut_requestBuffer);
		while (req->isTaken)
		{
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
		printf("Thread %d, Received request from client %d\n", myThreadID, myRequest.clientID);

		int numSeats = myRequest.numSeats;
		sem_t *semArray[numSeats];
		//   Request req;
		//   char* infoBuf;

		for (int i = 0; i < numSeatsToBook; i++) //Criação do grupo de semaforos
		{
			sprintf(str, "%d", req.seatsPreferences[i]);
			char str[12];

			if (semArray[i] == SEM_FAILED)
				semArray[i] = sem_open(str, O_RDWR);
		}

		if (infoTicket->room.numberSeats == infoTicket->room.occupiedSeats) //the room is full
		{
			*((int *)retValue) = 6;
			return retValue;
		}

		for (int i = 0; i < numSeatsToBook; i++) //lock dos semaforos
			sem_wait(semArray[i]);

		for (int i = 0; i < numSeatsToBook; i++)
		{
			if (infoTicket->room.seats[req.seatsPreferences[i]].status == FREE)
			{
				infoTicket->room.seats[req.seatsPreferences[i]].status = BOOKED;
				infoTicket->room.occupiedSeats++;
				bookedSeats[numBookedSeats] = req.seatsPreferences[i];
				;
				numBookedSeats++;
			}
			if (numBookedSeats == req.numSeats)
			{
				validRequest = 1;
				break;
			}
		}

		if (!validRequest) //the request was not completed
		{
			for (int h = 0; h < bookedSeats; h++) //free booked seats
				infoTicket->room.seats[freeSeats[h]].status = FREE;

			for (int j = 0; j < numSeats; j++) //unlock dos semaforos
				sem_post(semArray[j]);

			*((int *)retValue) = 5;
			return retValue;
		}
		else //the request was successful
		{
			for (int j = 0; j < numSeats; j++) //unlock dos semaforos
				sem_post(semArray[j]);
		}
	}
	printf("Thread %d realised that it's time to close doors!\n", myThreadID);
	return NULL;
}
*/

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
