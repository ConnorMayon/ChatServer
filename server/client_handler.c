// header files
#include "client_handler.h"

// constants

// function to talk to a client
void* talk_to_client(void *_args)
{
	// initialize varaibles
	Message* inputMessage = (Message*) malloc(sizeof(Message));
	Message* outputMessage = (Message*) malloc(sizeof(Message));
	struct args* args = (struct args*) _args;
	int clientSocket = args->clientSocket;
	ChatNodeLL *headPtr = args->chatroomList;

	// lock mutex
	pthread_mutex_unlock(&clientsMutex);

	// create a pointer for the chatroom list
	ChatNodeLL *ptr = headPtr;
	
	if(clientSocket == NULL)
	{
		close(clientSocket);
		pthread_exit(EXIT_FAILURE);
	}
	
	// while the client is connected
	while(1)
	{
		// read the message sent from the client in the stages sent
		receive_message_from_server(clientSocket, inputMessage);

		// set the ptr to the (null) head
		ptr = headPtr;
		
		// determine what identifier was sent in the message and start switch statement
		switch(inputMessage->messageType)
		{
			// if the JOIN identifier was sent
			case JOIN:
				// empty print statement because otherwise the first line of code is the newNode code and the compiler doesn't like that :(
				printf("");
				
				// connect the client to the chatroom by adding them to the chat node.
					// create the new chat node to add later
				ChatNode* newNode = create_chat_node(inputMessage->chatNode.ip, inputMessage->chatNode.portNum, inputMessage->chatNode.logName);
				
					// add the new node's thread number manually
				newNode->threadNum = clientSocket;
				
					// add the new clinet to the chat node list
				add_chat_node(ptr, newNode);
				
				
					// set the new node as the first bounds if not already set
				if(args->bounds->firstNode == NULL) args->bounds->firstNode = newNode;
				
					// otherwise set the new node as the last bounds
				else args->bounds->lastNode = newNode;

				// form the message struct so we can send a message to all the other clients
				outputMessage = create_message(JOIN, inputMessage->note, newNode);

				// send the join message to the chatroom for the requesting client
				while(ptr->nextNode != NULL)
				{
    					// go to the next chat node
					ptr = ptr->nextNode;
					
					// if the selected chat node doesn't equal the newly created node
					if(ptr->chatNode != newNode)
					{
						
						// write the message to the current chat node
						send_message_to_server(ptr->chatNode->threadNum, outputMessage);
					}
				}
				
				// end of this case
				break;				
				
			// if the LEAVE identifier was sent
			case LEAVE:
				
				// move past the null head
				ptr = ptr->nextNode;

				// determine sender
				while(strcmp(ptr->chatNode->logName, inputMessage->chatNode.logName) != 0)
				{
					// go to the next chat node
					ptr = ptr->nextNode;
				}
				
				// form the message struct so we can send a message to all the other clients
				outputMessage = create_message(LEAVE, inputMessage->note, &inputMessage->chatNode);

				// reset the pointer to the head.
				ptr = headPtr;

				// send the leave message to the rest of the chatroom
				while(ptr->nextNode != NULL)
				{
					// go to the next chat node
					ptr = ptr->nextNode;
					
					// if the selected chat node is not the leaving node
					if(strcmp(ptr->chatNode->logName, inputMessage->chatNode.logName) != 0 )
					{
						
						// write the message to the current chat node
						send_message_to_server(ptr->chatNode->threadNum, outputMessage);
					}
				}
				
				// remove the chat node
				remove_chat_node(headPtr, &ptr->chatNode);
				
				// close the thread between the sender and server
				close(clientSocket);
				pthread_exit(EXIT_SUCCESS);
				
			// if the SHUTDOWN ALL identifier was sent
			case SHUTDOWN_ALL:
				
				// form the message struct so we can send a message to all the other clients before shutting down
				outputMessage = create_message(SHUTDOWN_ALL, inputMessage->note, &inputMessage->chatNode);
				
				// send the leave message to the entire chatroom
				while(ptr->nextNode != NULL)
				{
					// go to the next chat node
					ptr = ptr->nextNode;
					
					// write the message to the current chat node
					send_message_to_server(ptr->chatNode->threadNum, outputMessage);
				}
				
				// shutdown the server
				exit(EXIT_SUCCESS);
				
			// if the NOTE identifier was sent
			case NOTE:
				
				// form the message struct so we can send the note to all the other clients before shutting down
				outputMessage = create_message(NOTE, inputMessage->note, &inputMessage->chatNode);

				// send the message to everyone but the sender
				while(ptr->nextNode != NULL)
				{
					// move to the next node
					ptr = ptr->nextNode;
					
					// make sure the current node isn't the sender
					if(strcmp(ptr->chatNode->logName, inputMessage->chatNode.logName) != 0)
					{
						// write the message to the current chat node
						send_message_to_server(ptr->chatNode->threadNum, outputMessage);
					}
				}
				
				// end of this case
			// end of the switch statement
		}
	}
	// this will never be reached
	//return NULL;
// end of function
}
