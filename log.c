#include "log.h"

FILE* sBookFile;
FILE* sLogFile;
FILE* cBookFile;


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
int openFiles(){

    if((sBookFile = fopen(SERVER_BOOK_FILENAME, "a")) == NULL){
        return ERROR_OPENING_SBOOK_FILE;
    }

    if((sLogFile = fopen(SERVER_LOG_FILENAME, "a")) == NULL){
        return ERROR_OPENING_SLOG_FILE;
    }

    if((cBookFile = fopen(CLIENT_BOOK_FILENAME, "a")) == NULL){
        return ERROR_OPENING_CBOOK_FILE;
    }
    return FILES_OPENED_WITH_SUCCESS;
}

void clientLogBookSuccess(int clientID, int num_booked_seats, int *booked_seats) {
    FILE* cLogFile = fopen(CLIENT_LOG_FILENAME, "a");

    for(int i = 1; i <= num_booked_seats; i++) {
        fprintf(cLogFile, "%0*d %0*d.%0*d %0*d\n",
            WIDTH_PID, clientID,
            (WIDTH_XXNN-1)/2, i,
            (WIDTH_XXNN-1)/2, num_booked_seats,
            WIDTH_SEAT, booked_seats[i-1]);
    }

    fclose(cLogFile);
}

void clientLogBookFailed(int clientID, int error) {
    FILE* cLogFile = fopen(CLIENT_LOG_FILENAME, "a");
    fprintf(cLogFile, "%0*d %s\n", WIDTH_PID, clientID, getErrorDescription(error));
    fclose(cLogFile);
}

void openTicketOfficeLog(int toNumber){
    fprintf(sLogFile, "%04d-OPEN\n", toNumber);
}

void closeTicketOfficeLog(int toNumber){
    fprintf(sLogFile, "%04d-CLOSED\n", toNumber);
}

void serverLogSuccess(int toNumber, int clientID, int numSeats, int* preferedSeats, int* booked_seats){

    int numSpaces = MAX_CLI_SEATS*5;
    int preferedSeatsSize = sizeof(preferedSeats)/sizeof(preferedSeats[0]);

    fprintf(sLogFile, "%02d-%i-%02d:", toNumber, clientID, numSeats);

    // Print the prefered Seats
    for(int i = 0; i < preferedSeatsSize; i++){
        fprintf(sLogFile, " %04d", preferedSeats[i]);
        numSpaces -= 5;
    }

    // Fill the spaces to be formated
    for(int i = numSpaces; i > 0; i--){
        fprintf(sLogFile, " ");
    }

    fprintf(sLogFile, " -");

    for(int i = 0; i < numSeats; i++){
        fprintf(sLogFile, " %04d", booked_seats[i]);
    }

    fprintf(sLogFile, "\n");
}

void serverLogFailure(int toNumber, int clientID, int numSeats, int* preferedSeats, int error){

    int numSpaces = MAX_CLI_SEATS*5;
    int preferedSeatsSize = sizeof(preferedSeats)/sizeof(preferedSeats[0]);

    fprintf(sLogFile, "%02d-%i-%02d:", toNumber, clientID, numSeats);

    // Print the prefered Seats
    for(int i = 0; i < preferedSeatsSize; i++){
        fprintf(sLogFile, " %04d", preferedSeats[i]);
        numSpaces -= 5;
    }

    // Fill the spaces to be formated
    for(int i = numSpaces; i > 0; i--){
        fprintf(sLogFile, " ");
    }

    fprintf(sLogFile, " - %s\n", getErrorDescription(error));

}

void closeServerLog(){
    fprintf(sLogFile, "SERVER CLOSED");
}

void fillServerBookings(Room room){
    for(int i = 0; i < room.numSeats; i++){
        if(room.seats[i].status == BOOKED){
            fprintf(sBookFile, "%04d\n", i+1);  //i+1 é o lugar, pois a sala começa no lugar 1
        }
    }
}