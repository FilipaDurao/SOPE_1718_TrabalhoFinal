#include "log.h"

FILE* sBookFile;
FILE* sLogFile;
FILE* cBookFile;
FILE* cLogFile;

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

    if((cLogFile = fopen(CLIENT_LOG_FILENAME, "a")) == NULL){
        return ERROR_OPENING_CLOG_FILE;
    }

    return FILES_OPENED_WITH_SUCCESS;
}

int clientLogBookSuccess(int clientID) {

}

int clientLogBookFailed(int clientID) {

}