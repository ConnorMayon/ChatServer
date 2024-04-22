#include "receiver_handler.h"

// function that handles recieving from client
void *runReceiving(void *_args)
    {
    struct conn_args *args = (struct conn_args *) _args;
    char input[1000];
    char c;
    Message* sendMessage;

    while(args->active)
        {
        while(args->connected)
            {
            // while connected to a server

            // reads server output
            

            // read data into local struct using read function

            switch(c)
                {
                // translate control code
                case 'J':
                // if code is J
                    // print JOIN
                    printf(JOINED_COLOR);
                    printf("Someone joined chat\n");
                    printf(RESET_COLOR);
                break;
                case 'L':
                // if code is L
                    // print LEAVE
                    printf(LEFT_COLOR);
                    printf("Someone left chat\n");
                    printf(RESET_COLOR);
                break;
                case 'N':
                // if code is N
                    // print message
                    printf(NOTE_COLOR);
                    printf("Someone:");
                    printf(RESET_COLOR);
                    printf("Message here\n");
                break;
                case 'S':
                // if code is S
                    // print message
                    printf("Shutting down");
                    // run shutdown
                    args->connected = false;
                    args->active = false;
                    // exit(EXIT_SUCCESS);
                break;
                }
            
            clientActive = args->active;
            clientConnected = args->connected;
            }
        }
    return NULL;
    }