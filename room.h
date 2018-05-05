/**
 * @brief A library that handles an event's room composed by rooms where clients that book seats
 * 
 * @file room.h
 */

////////////////////////////////////////////////////
////////////////////////SEAT////////////////////////
////////////////////////////////////////////////////

typedef enum {FREE, BOOKED} seatSt;

typedef struct {
    int number; // identifies the seat [1...N]
    int clientID; // the client who booked this seat (if the seat is free, this value has no meaning)
    seatSt status; // flag that indicates if this seat is booked or not 
} Seat;

/**
 * @brief Create a Seat object (set as free)
 * 
 * @param number The Seat number (identifier)
 * @return seat 
 */
Seat createSeat(int number);


////////////////////////////////////////////////////
////////////////////////ROOM////////////////////////
////////////////////////////////////////////////////

typedef struct {
    int numberSeats; // this room capacity
    Seat* seats; // the room seats
} Room;

/**
 * @brief Create a Room object and initializes the seat list. All seats are set as FREE
 * 
 * @param numberSeats The room capacity
 * @return room* 
 */
Room createRoom(int numberSeats);

/**
 * @brief Deletes a room
 * Note: This is needed to free dynamically allocated memory
 * 
 * @param r The room to be deleted
 */
void deleteRoom(Room r);

/**
 * @brief Tells if a specific seat is free
 * 
 * @param seats List of seats
 * @param seatNum The seat identifier
 * @retval 1 The seat is free
 * @retval 0 The seat is booked
 */
int isSeatFree(Seat *seats, int seatNum);

/**
 * @brief Sets a seat as booked by some client
 * 
 * @param seats List of seats
 * @param seatNum The seat identifier
 * @param clientID The client identifier who booked the seat
 */
void bookSeat(Seat *seats, int seatNum, int clientID);

/**
 * @brief Sets a seat as free
 * 
 * @param seats List of seats
 * @param seatNum The seat identifier
 */
void freeSeat(Seat *seats, int seatNum);