#include "log.h"

FILE* sBookFile;
FILE* sLogFile;
FILE* cBookFile;
FILE* cLogFile;

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

int clientLogBookFailed(int clientID, struct) {

}