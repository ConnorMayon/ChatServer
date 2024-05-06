#include "receiver_handler.h"



// function that handles recieving from client
void *run_receiving()
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
            // reads server output
            receive_message_from_server(clientSocket, receiveMessage);
            if(clientConnected)
				{
				ChatNode thisNode = receiveMessage->chatNode;
				switch(receiveMessage->messageType)
					{
					// translate control code
					case JOIN:
					// if code is J
						// print JOIN
						printf(JOINED_COLOR);
						printf("%s joined chat", thisNode.logName);
						printf(RESET_COLOR);
						printf("\n");
					break;
					case LEAVE:
					// if code is L
						// print LEAVE
						printf(LEFT_COLOR);
						printf("%s left chat", thisNode.logName);
						printf(RESET_COLOR);
						printf("\n");
					break;
					case NOTE:
					// if code is N
						// print message
						printf(NOTE_COLOR);
						receiveMessage->note[strlen(receiveMessage->note)-1] = '\0';
						printf("%s:", thisNode.logName);
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
        }
    free(receiveMessage);
    return NULL;
    }
