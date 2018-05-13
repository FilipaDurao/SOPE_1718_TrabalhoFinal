#include "officeTicket.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

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

		// release some resources
		free(req->seatsPreferences);
	}

	pthread_exit(NULL);
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
