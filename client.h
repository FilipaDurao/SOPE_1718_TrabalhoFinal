#define FIFO_CREATION_ERROR 1


#define PID_LENGTH 10
/**
 * @brief Creates a FIFO to get the answer from the client
 * The FIFO name is ansXXXX where XXXX is the client PID
 * 
 * @return int 
 */
int createFIFO();

/**
 * @brief Sends the request to the client through a FIFO called requests
 * The format is the following:
 * <client ID> <num_wanted_seats> <pref_seat_list>*
 * The * indicates end of request
 */
void sendRequest();

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