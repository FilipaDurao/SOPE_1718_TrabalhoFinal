#define FIFO_CREATION_ERROR 1
#define OPEN_SERVER_FIFO_ERROR 2
#define WRITE_SERVER_FIFO_ERROR 3

#define PID_LENGTH 10
#define REQUEST_LENGTH 256
#define SERVER_REQUEST_FIFO "request"
/**
 * @brief Creates a FIFO to get the answer from the client
 * The FIFO name is ansXXXX where XXXX is the client PID
 * 
 * If the fifo creation fails, ends the program
 */
void createFIFO();

/**
 * @brief Sends the request to the client through a FIFO called requests
 * The format is the following:
 * <client ID> <num_wanted_seats> <pref_seat_list>*
 * The * indicates end of request
 */
void sendRequest(int num_wanted_seats, char* pref_seat_list);

/**
 * @brief Handler for SIG_ALARM
 * When a SIG_ALARM is raised, that indicates a time out and the client stops waiting for server answer
 *
 * @param signal 
 */
void timeoutHandler(int signal);

/**
 * @brief Gets the server answer, if any
 * 
 */
int getServerAnswer();