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

#define WIDTH_SEAT 4
#define WIDTH_PID 5
#define WIDTH_XXNN 5

int openFiles();


/**
 * @brief Logs a successful event on clog.txt
 * 
 * @param clientID 
 * @param num_booked_seats 
 * @param booked_seats 
 */
void clientLogBookSuccess(int clientID, int num_booked_seats, int *booked_seats);

/**
 * @brief Logs an error event on clog.txt
 * 
 * @param clientID 
 * @param error 
 */
void clientLogBookFailed(int clientID, int error);