#include "log.h"
#include <stdio.h>
#include <string.h>
#include "constants.h"
FILE* sBookFile;
FILE* sLogFile;

#define BUFFER_MAX_SIZE 4096

/**
 * @brief Maps an error code to an error abreviation string
 *
 * @param error The error code
 */
static char* getErrorDescription(int error) {
    switch(error) {
        case -1:
            return "MAX";

        case -2:
            return "NST";

        case -3:
            return "IID";

        case -4:
            return "ERR";

        case -5:
            return "NAV";

        case -6:
            return "FUL";

        case -7:
            return "OUT";

        default:
            return "";
    }
}

int openServerFiles() {
    if((sBookFile = fopen(SERVER_BOOK_FILENAME, "a")) == NULL){
        return ERROR_OPENING_SBOOK_FILE;
    }

    if((sLogFile = fopen(SERVER_LOG_FILENAME, "a")) == NULL){
        return ERROR_OPENING_SLOG_FILE;
    }
}

void closeServerFiles() {
    fclose(sBookFile);
    fclose(sLogFile);
}

void clientLogBookSuccess(int clientID, int num_booked_seats, int *booked_seats) {
    FILE* cLogFile = fopen(CLIENT_LOG_FILENAME, "a");

    char tempString[20];
    char finalString[BUFFER_MAX_SIZE];

    for(int i = 1; i <= num_booked_seats; i++) {
        sprintf(tempString, "%0*d %0*d.%0*d %0*d\n",
            WIDTH_PID, clientID,
            (WIDTH_XXNN-1)/2, i,
            (WIDTH_XXNN-1)/2, num_booked_seats,
            WIDTH_SEAT, booked_seats[i-1]);
        strcat(finalString, tempString);
        printf(" %d ", booked_seats[i-1]);
    }
    printf("\n");

    fprintf(cLogFile, "%s", finalString);

    fclose(cLogFile);
}

void clientLogBookFailed(int clientID, int error) {
    FILE* cLogFile = fopen(CLIENT_LOG_FILENAME, "a");
    fprintf(cLogFile, "%0*d %s\n", WIDTH_PID, clientID, getErrorDescription(error));
    fclose(cLogFile);
}

void clientLogBooking(int *list_bookings, int num_bookings) {
    FILE* cBookFile = fopen(CLIENT_BOOK_FILENAME, "a");
    for(int i = 0; i < num_bookings; i++) {
        fprintf(cBookFile, "%d\n", list_bookings[i]);
    }
}

void serverLogSuccess(int toNumber, int clientID, int numSeats, int* preferedSeats, int* booked_seats){

    int numSpaces = MAX_CLI_SEATS*5;
    int preferedSeatsSize = sizeof(preferedSeats)/sizeof(preferedSeats[0]);

    char tempString[20];
    char finalString[BUFFER_MAX_SIZE];

    sprintf(tempString, "%02d-%i-%02d:", toNumber, clientID, numSeats);
    strcat(finalString, tempString);

    // Print the prefered Seats
    for(int i = 0; i < preferedSeatsSize; i++){
        sprintf(tempString, " %04d", preferedSeats[i]);
        strcat(finalString, tempString);
        numSpaces -= 5;
    }

    // Fill the spaces to be formated
    for(int i = numSpaces; i > 0; i--){
        sprintf(tempString, " ");
        strcat(finalString, tempString);
    }

    sprintf(tempString, " -");
    strcat(finalString, tempString);
    
    numSpaces = MAX_CLI_SEATS*5;
    for(int i = 0; i < numSeats; i++, numSeats -= 5){
        sprintf(tempString, " %04d", booked_seats[i]);
        strcat(finalString, tempString);
    }

    // Fill the spaces to be formated
    for(int i = numSpaces; i > 0; i--){
        sprintf(tempString, " ");
        strcat(finalString, tempString);
    }

    sprintf(tempString, "\n");
    strcat(finalString, tempString);

    fprintf(sLogFile, "%s", finalString);
}

void serverLogFailure(int toNumber, int clientID, int numSeats, int* preferedSeats, int error){

    int numSpaces = MAX_CLI_SEATS*5;
    int preferedSeatsSize = sizeof(preferedSeats)/sizeof(preferedSeats[0]);

    char tempString[20];
    char finalString[BUFFER_MAX_SIZE];

    sprintf(tempString, "%02d-%i-%02d:", toNumber, clientID, numSeats);
    strcat(finalString, tempString);

    // Print the prefered Seats
    for(int i = 0; i < preferedSeatsSize; i++){
        sprintf(tempString, " %04d", preferedSeats[i]);
        strcat(finalString, tempString);
        numSpaces -= 5;
    }

    // Fill the spaces to be formated
    for(int i = numSpaces; i > 0; i--){
        sprintf(tempString, " ");
        strcat(finalString, tempString);
    }

    sprintf(tempString, " - %s\n", getErrorDescription(error));
    strcat(finalString, tempString);

    fprintf(sLogFile, "%s", finalString);

}

void openTicketOfficeLog(int toNumber){
    fprintf(sLogFile, "%02d-OPEN\n", toNumber);
}

void closeTicketOfficeLog(int toNumber){
    fprintf(sLogFile, "%02d-CLOSED\n", toNumber);
}

void closeServerLog(){
    fprintf(sLogFile, "SERVER CLOSED");
}

void fillServerBookings(int *seats, int numSeats){
    for(int i = 0; i < numSeats; i++){
        fprintf(sBookFile, "%04d\n", seats[i]); 
    }
}
