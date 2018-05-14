#define SERVER_BOOK_FILENAME        "sbook.txt"
#define SERVER_LOG_FILENAME         "slog.txt"
#define CLIENT_BOOK_FILENAME        "cbook.txt"
#define CLIENT_LOG_FILENAME         "clog.txt"

#define FILES_OPENED_WITH_SUCCESS    0
#define ERROR_OPENING_SBOOK_FILE    -1
#define ERROR_OPENING_SLOG_FILE     -2
#define ERROR_OPENING_CBOOK_FILE    -3
#define ERROR_OPENING_CLOG_FILE     -4
/**
 * @brief Opens the files used by server to log
 * 
 * @return int 
 */
int openServerFiles();

/**
 * @brief Closes the opened files
 * 
 */
void closeServerFiles();

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

/**
 * @brief meh
 * 
 * @param list_bookings 
 * @param num_bookings 
 */
void clientLogBooking(int *list_bookings, int num_bookings);

/**
 * @brief Logs a successful request on slog.txt
 *
 * @param toNumber
 * @param clientID
 * @param numSeats
 * @param preferedSeats
 * @param booked_seats
 */
void serverLogSuccess(int toNumber, int clientID, int numSeats, int* preferedSeats, int* booked_seats);

/**
 * @brief Logs a failed request on slog.txt
 *
 * @param toNumber
 * @param clientID
 * @param numSeats
 * @param preferedSeats
 * @param error
 */
void serverLogFailure(int toNumber, int clientID, int numSeats, int* preferedSeats, int error);


/**
 * @brief Logs the opening of a ticket office on slog.txt
 *
 * @param toNumber
 */
void openTicketOfficeLog(int toNumber);

/**
 * @brief Logs the closing of a ticket office on slog.txt
 *
 * @param toNumber
 */
void closeTicketOfficeLog(int toNumber);

/**
 * @brief does something
 * 
 */
void closeServerLog();

/**
 * @brief Writes all the booked seats in sbook.txt
 *
 * @param room
 */
void fillServerBookings(int *seats, int numSeats);
