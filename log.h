#include <stdio.h>

#define SERVER_BOOK_FILENAME        "sbook.txt"
#define SERVER_LOG_FILENAME         "slog.txt"
#define CLIENT_BOOK_FILENAME        "cbook.txt"
#define CLIENT_LOG_FILENAME         "clog.txt"

#define FILES_OPENED_WITH_SUCCESS    0
#define ERROR_OPENING_SBOOK_FILE    -1
#define ERROR_OPENING_SLOG_FILE     -2
#define ERROR_OPENING_CBOOK_FILE    -3
#define ERROR_OPENING_CLOG_FILE     -4

int openFiles();
