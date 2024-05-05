#include "receiver_handler.h"



// function that handles recieving from client
void *runReceiving()
    {
    Message* receiveMessage = (Message*) malloc(sizeof(Message));
    bool temp;
    while(true)
        {
        if(!clientConnected)
		{
		temp = clientConnected;
		}
        // while connected to a server
        while(clientConnected)
            {
            printf("Waiting for recieve!\n");
            // reads server output
            receive_message_from_server(clientSocket, receiveMessage);
	    ChatNode thisNode = receiveMessage->chat_node;
	    printf("%s", thisNode.log_name);
            switch(receiveMessage->message_type)
                {
                // translate control code
                case JOIN:
                // if code is J
                    // print JOIN
                    printf(JOINED_COLOR);
                    printf("%s joined chat", thisNode.log_name);
		    printf(RESET_COLOR);
		    printf("\n");
                break;
                case LEAVE:
                // if code is L
                    // print LEAVE
                    printf(LEFT_COLOR);
                    printf("%s left chat", thisNode.log_name);
		    printf(RESET_COLOR);
		    printf("\n");
                break;
                case NOTE:
                // if code is N
                    // print message
                    printf(NOTE_COLOR);
                    printf("%s:", thisNode.log_name);
                    printf("%s", receiveMessage->note);
		    printf(RESET_COLOR);
		    printf("\n");
                break;
                case SHUTDOWN_ALL:
                // if code is S
                    // print message
                    printf("Shutting down");
                    // run shutdown
                    exit(EXIT_SUCCESS);
                break;
                }
            
            }
        }
    free(receiveMessage);
    return NULL;
    }
