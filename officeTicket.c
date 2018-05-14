#include "officeTicket.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

static int officeTicketID = 1;

void *enableOfficeTicket(void *arg)
{
	// set a thread ID
	int myOfficeTicketID = officeTicketID++;
	printf("Hello from thread %d\n", myOfficeTicketID);

	// cast the argument and separate some of content for easier access
	officeTicketInfo *info = (officeTicketInfo *)arg;
	Request *req = info->request;

	while (1)
	{
		// is there any pendent request ?
		if (sem_wait(info->sem_buffer_full)){
			perror(NULL);
		}
		printf("Thread %d got access to request buffer!\n", myOfficeTicketID);
		// the server may indicate there are pendent requests to unlock threads so that they can terminate
		if (*(info->isTimeOut))
			break;

		// copy the request from the buffer. releasing the buffer as fast as possible
		Request myRequest = *req;
		printf("Thread %d got the request from client %d\n", myOfficeTicketID, myRequest.clientID);
		// set buffer as empty
		if (sem_post(info->sem_buffer_empty)){
			perror(NULL);
		}

		// validate request before process it
		int req_status = isValidRequest(req, &info->room);
		printf("Request from %d, code %d\n", req->clientID, req_status);

		if (req_status < 0) {
			// the request is invalid, send the answer to client
			answerClient(&myRequest, NULL);
		}
		else { 
			int *list_booked_seats = processRequest(info);
			if(list_booked_seats == NULL)
				printf("Request from %d, failed!\n", req->clientID);
			else
				printf("Request from %d, OK!\n", req->clientID);

			answerClient(req, list_booked_seats);

			if(list_booked_seats != NULL) 
				free(list_booked_seats);
		}

		// release some resources
		free(req->seatsPreferences);
	}

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

	if (request->numSeats < 0){
		return -4; // Numero de lugares invalido (outros erros em parametros)
	}

	return 1;
}

int* processRequest(officeTicketInfo *info)
{
	Request *req = info->request;
	Room *room = &info->room;

	// there can only be one thread locking seats at each time
	// for that reason, there's the sem_access semaphore
	sem_wait(info->sem_access);

	// try to lock the seats mentioned on preference list
	for (int i = 0; i < req->numSeatsPreferences; i++) {
		int seat_id = req->seatsPreferences[i];
		sem_wait(room->seats[seat_id - 1].sem_unlocked);
	}

	printf("Could lock all seats on pref list!\n");

	// unlock access to seats
	sem_post(info->sem_access);

	// with the requested seats all locked, let's book them if possible
	int booked_seats = 0; // counter
	int *list_booked_seats = malloc(sizeof(int) * req->numSeats); // list that will contain the booked seats
	
	for (int i = 0; i < req->numSeatsPreferences && booked_seats < req->numSeats; i++) {
		int seat_id = req->seatsPreferences[i];

		if (isSeatFree(room->seats, seat_id)) {
			// it's free, book it
			bookSeat(room->seats, seat_id, req->clientID);
			// add seat identifier to the list
			list_booked_seats[booked_seats++] = seat_id;
		}
	}
	// if the request failed, free the seats
	if (booked_seats != req->numSeats)
		for (int i = 0; i < booked_seats; i++){
			int seat_num = list_booked_seats[i];
			Seat *seats = room->seats;
			freeSeat(seats, seat_num);
		}

	// unlock seat semaphores
	for (int i = 0; i < req->numSeatsPreferences; i++) {
		int seat_id = req->seatsPreferences[i];
		sem_post(room->seats[seat_id - 1].sem_unlocked);
	}

	// if request successful return the list of seats
	if (booked_seats == req->numSeats)
		return list_booked_seats;
	else {
		free(list_booked_seats);
		return NULL;
	}
}

void answerClient(Request *req, int *list_booked_seats)
{
	// generate fifo name
	char fifoName[10];
	sprintf(fifoName, "ans%d", req->clientID);

	// open fifo
	int fd = open(fifoName, O_WRONLY);
	printf("thread opening %s", fifoName);

	// write answer on fifo
	if (list_booked_seats == NULL)
	{
		// the request was not successful
		// write a negative number
		printf("Failed request, writing a single number!\n");
		int out = -1;
		write(fd, &out, sizeof(int));
	}
	else
	{
		printf("Successful request, writing chunck of data\n");

		if(write(fd, &req->numSeats, sizeof(int)) <= 0) {
			if(errno) 
				perror(NULL);
		}
		if(write(fd, list_booked_seats, sizeof(int)*req->numSeats) <= 0) {
			if(errno) 
				perror(NULL);
		}

		close(fd);
	}

	
}
