# SOPE_1718_TrabalhoFinal

# Server
Handles the client requests.

The server is responsible for:
1. Creating a FIFO to receive the client requests
2. Create threads that will process the clients requests (ticket offices)
3. Load the client requests from the FIFO **requests** and load it to a unit buffer shared between threads
4. When timeout, the server must close the **requests** fifo and tell threads to stop processing

## The structure
The office tickets need to know/access:

- List of room seats
- A flag that tells if the office tickets should close 
- The unit buffer where the server loads the client requests

Therefore, the structure could be something like:
```C
typedef struct {
    int numOfficeTickets;
    char* requestBuffer;
    bool closeOfficeTickects;
    Room room;
} Server;
```
So, the threads must be able to access this structure. It can be a static global variable on `server.c`

## How will threads know they must stop?
Probably the cleanest solution is to use signals, `SIGALARM`.

When the server is created, on it's "constructor", it installs the handler for `SIGALARM`. The only thing it needs to do is update the `closeOfficeTickects` flag. The threads, which can access the flag, before trying to handle a request verify this flag. If it's false, try to read the buffer, else return to end.

## Comunication between server and office tickets
The buffer is unitary. We need to guarantee that the same request is not handled multiple times, it would result on the same client requesting the same seats a certain number of times. Of course, the next section prevents that things go wrong. One of the threads would book the seats for the client, and the others would fail because the requested seats are already booked (by the same client). But, that means the client would receive one successful answer, and multiple failed answers, which is.. stupid. Moreover, this would waste CPU time.

Therefore, we need to sync the server and office tickets. Using a mutex and a condition variable (optimization) should do the job.

So, we need a mutex, `mut_unitBuffer`, and a condition variable, `cvar_unitBuffer`, shared between threads and server. They can be static global variables on `server.c`.

1. Initially the buffer is empty (NULL)
2. The server locks the mutex and checks if there are pendent requests in the unitary buffer. In other words, if it's not null, do nothing, as the requests was not catched by any thread.
3. Otherwise, check if there are any requests on the FIFO **requests**.
    - If so, updates the buffer and tells threads "Hey, I updated the buffer" through the call pthread_cond_signal(&cvar_unitBuffer)
    - Else, it unlocks the mutex but the buffer remains NULL, and threads, unless already doing something, will keep waiting

The threads, on the other hand, must:
1. Lock the mutex
2. Check if the unit buffer is NULL
    - If so, waits, pthread_cond_wait(&cvar_unitBuffer, &mut_unitBuffer)
    - Else processes the request and updates the buffer to NULL value

## Comunication between office tickets
On the previous section we planned how the server and office tickets correctly comunicate. When a office ticket is processing a client request, we need to update the seats status, hence this is another criticial area that needs a mutex and a conditional variable (to save CPU resources).

The process is similar. 

Hmmm i need to rethink this.. because..