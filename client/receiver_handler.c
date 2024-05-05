#include "receiver_handler.h"



// function that handles recieving from client
void *runReceiving(void *_args)
    {
    struct conn_args *args = (struct conn_args *) _args;
    Message* receiveMessage = (Message*) malloc(sizeof(Message));
    while(true)
        {
        // while connected to a server
        while(args->connected)
            {
            // reads server output
            receive_message_from_server(clientSocket, receiveMessage);
	    ChatNode thisNode = receiveMessage->chat_node;
	    printf("%s", thisNode.log_name);
	    pthread_mutex_lock(&mutex);
            switch(receiveMessage->message_type)
                {
                // translate control code
                case 'J':
                // if code is J
                    // print JOIN
                    printf(JOINED_COLOR);
                    printf("%s joined chat\n", thisNode.log_name);
                break;
                case 'L':
                // if code is L
                    // print LEAVE
                    printf(LEFT_COLOR);
                    printf("%s left chat\n", thisNode.log_name);
                break;
                case 'N':
                // if code is N
                    // print message
                    printf(NOTE_COLOR);
                    printf("%s:", thisNode.log_name);
                    printf("Message here\n");
                break;
                case 'S':
                // if code is S
                    // print message
                    printf("Shutting down");
                    // run shutdown
                    args->connected = false;
                    exit(EXIT_SUCCESS);
                break;
                }
            
            printf(RESET_COLOR);
            clientConnected = args->connected;
            pthread_mutex_unlock(&mutex);
            }
        }
    free(receiveMessage);
    return NULL;
    }
