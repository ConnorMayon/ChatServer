#include "receiver_handler.h"

// function that handles recieving from client
void *runReceiving(void *_args)
    {
    printf("\nRECEIVING!\n");
    // while connected to a server
        // reads server output

        // read data into local struct using read function

        // translate control code
            // if code is J
                // print JOIN
            // if code is L
                // print LEAVE
            // if code is N
                // print message
            // if code is S
                // print message
                // run shutdown
    return NULL;
    }