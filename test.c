#include "room.h"
#include <stdio.h>

int testNum = 1;

void assert_equal(int expected, int ret) {
    if(expected == ret) {
        printf("Test %d (OK)\n", testNum++);
    }
    else {
        printf("Test %d (FAILED)\n", testNum++);
    }
}


void testSeats() {
    testNum = 0;

    printf("\n\nSEATS\n\n");
    
    // Testing seat initialization
    Seat s = createSeat(10);

    assert_equal(10, s.number);
    assert_equal(FREE, s.status);

    // Test Room
    Room r = createRoom(100);

    assert_equal(1, isSeatFree(r.seats, 1));
    assert_equal(1, isSeatFree(r.seats, 50));

    // Test book seat
    bookSeat(r.seats, 50, 123);
    assert_equal(BOOKED, r.seats[49].status);
    assert_equal(123, r.seats[49].clientID);
    assert_equal(0, isSeatFree(r.seats, 50));

    // Test free seat
    freeSeat(r.seats, 50);
    assert_equal(FREE, r.seats[49].status);
    assert_equal(1, isSeatFree(r.seats, 50));

    // Test deleting room (ensure there's no memory leak with valgrind)
    deleteRoom(r);
}

int main() {
    testSeats();
}