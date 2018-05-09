#include "officeTicket.h"

void *enableOfficeTicket(void *info)
{
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

int isSeatFree(Seat *seats, int seatNum){}

void bookSeat(Seat *seats, int seatNum, int clientID){}

void freeSeat(Seat *seats, int seatNum){}

void answerClient(){}

void logSuccessRequest();

void logFailedSuccess();
