#define FIFO_CREATION_ERROR 1
#define OPEN_SERVER_FIFO_ERROR 2
#define WRITE_SERVER_FIFO_ERROR 3
#define SIGALRM_ERROR 4
#define ERROR_OPEN_ANSWER_FIFO 5
#define ERROR_INVALID_ARGUMENTS 1

#define PID_LENGTH 5
#define REQUEST_LENGTH 256
#define SERVER_REQUEST_FIFO "request"

/**
 * @brief Creates a FIFO to get the answer from the client
 * The FIFO name is ansXXXX where XXXX is the client PID
 * 
 * If the fifo creation fails, ends the program
 */
char* createFIFO();

/**
 * @brief Sends the request to the client through a FIFO called requests
 * The format is the following:
 * <client ID> <num_wanted_seats> <pref_seat_list>*
 * The * indicates end of request
 */
void sendRequest(int num_wanted_seats, char* pref_seat_list);

/**
 * @brief Handler for SIG_ALARM
 * When a SIG_ALARM is raised, that indicates a time out and updates a flag
 *
 * @param signal 
 */
void timeoutHandler(int signal);

/**
 * @brief Gets the server answer, if any
 * 
 */
int getServerAnswer();