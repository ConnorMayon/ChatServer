#include "sender_handler.h"

// function that handles sending to client
void *runSending(void *_args)
    {
    // continuously reads client input

    // translantes client input into control code
        // if client said LEAVE
            // if client is connected
                // set code to L
                // set message to empty
                // send to server
                // end connection to client
            // if not connected
                // display error

        // if client said JOIN
            // if client is not connected
                // set code to J
                // set message to empty
                // connect to server
                // send to server
            // if client is connected
                // display error

        // if client said SHUTDOWN 
            // if client is connected
                // set code to L
                // set message to empty
                // send to server
            // handle shutdown

        // if client said SHUTDOWN ALL 
            // if client is connected
                // set code to S
                // set message to empty
                // send to server
            // if not connected
                // display error

        // if client said anything else
            // if client is connected
                // set code to N
                // set message to user input
                // send to server
            // if not connected
                // display error

    return NULL;
    }

// connects to server
void makeConnection(int client_socket)
    {
    // create addr struct

    // connect to server socket

    }