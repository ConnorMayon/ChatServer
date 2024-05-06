// header files
#include "client_handler.h"

// constants

// function to talk to a client
void* talk_to_client(void *_args)
{
	// initialize varaibles
//	char identifier;
//	char message[64];
//	char chatNodeip[15];
//	int chatNodePort;
//	char chatNodeName[16];
	Message* inputMessage = (Message*) malloc(sizeof(Message));
	Message* outputMessage = (Message*) malloc(sizeof(Message));
	struct args* args = (struct args*) _args;
	int clientSocket = args->clientSocket;

	// lock mutex
	pthread_mutex_unlock(&clients_mutex);

	// DEBUG: CHECK FOR PASSING UNLOCK
	printf("PASSED UNLOCK\n");
	
	// create a pointer for the chatroom list
	ChatNodeLL *ptr = args->chatroomList;

	// DEBUG: CHECK FOR PASSING LL
	printf("PASSED LL\n");
	
	if(clientSocket == NULL)
	{
		close(clientSocket);
		pthread_exit(EXIT_FAILURE);
	}
	// DEBUG: CHECK FOR PASSING LL
	printf("PASSED SOCKET CHECK\n");
	
	// while the client is connected
	while(1)
	{
		/* OLD READING CODE THAT WORKS 100% 
		// read the message sent from the client in the stages sent
			// read the identifier
		read(clientSocket, &identifier, sizeof(identifier));
	
			// read the message
		read(clientSocket, &message, sizeof(message));
		
			// read the sender ip
		read(clientSocket, &chatNodeip, sizeof(chatNodeip));
		
			// read the sender port
		read(clientSocket, &chatNodePort, sizeof(chatNodePort));
		
			// read the sender name
		read(clientSocket, &chatNodeName, sizeof(chatNodeName));
		*/

		/* NEW UNTESTED READING CODE */
		receive_message_from_server(clientSocket, inputMessage);
		
		// DEBUG: CHECK FOR PASSED READING
		printf("PASSED READING FROM THE CLIENT\n");
		
		// set the ptr to the (null) head
		ptr = args->chatroomList;
		
		// DEBUG: PRINT THAT A NEW CASE HAS BEGAN
		printf("\n\n\n====== NEW CASE ======\n");

		
		
		// determine what identifier was sent in the message and start switch statement
		switch(inputMessage->message_type)
		{
			// if the JOIN identifier was sent
			case JOIN:
				// DEBUG: CHECK IF FINDING CASE AND MAKING OUTPUT
				printf("CASE: JOIN");
				
				// lock mutex
				pthread_mutex_lock(&clients_mutex);

				// connect the client to the chatroom by adding them to the chat node.
					// create the new chat node to add later
				ChatNode* newNode = create_chat_node(inputMessage->chat_node.ip, inputMessage->chat_node.port_num, inputMessage->chat_node.log_name);
				
					// add the new node's thread number manually
				newNode->thread_num = clientSocket;
				
					// add the new clinet to the chat node list
				add_chat_node(ptr, newNode);
				
					// set the new node as the first bounds if not already set
				if(args->bounds->first_node == NULL) args->bounds->first_node = newNode;
				
					// otherwise set the new node as the last bounds
				else args->bounds->last_node = newNode;

				// form the message struct so we can send a message to all the other clients
				outputMessage = create_message(JOIN, inputMessage->note, newNode);
				
				// send the join message to the chatroom for the requesting client
				while(ptr->next_node != NULL)
				{
    					// go to the next chat node
					ptr = ptr->next_node;
					
					// if the selected chat node doesn't equal the newly created node
					if(!ptr->chat_node == newNode)
					{
						// write the message to the current chat node
						send_message_to_server(ptr->chat_node->thread_num, outputMessage);
					}
				}
				// lock mutex
				pthread_mutex_unlock(&clients_mutex);
				
				// end of this case
				break;				
				
			// if the LEAVE identifier was sent
			case LEAVE:
				// DEBUG: CHECK IF FINDING CASE
				printf("CASE: LEAVE\n");
				
				// move past the null head
				ptr = ptr->next_node;

				// determine sender
				while(strcmp(ptr->chat_node->log_name, inputMessage->chat_node.log_name) != 0)
				{
					// go to the next chat node
					ptr = ptr->next_node;
				}
				
				// form the message struct so we can send a message to all the other clients
				outputMessage = create_message(LEAVE, inputMessage->note, ptr->chat_node);
				
				// reset the pointer to the head.
				ptr = args->chatroomList;

				// DEBUG: START SENDING MESSAGE TO THE CHATROOM
				printf("SENDING MESSAGE TO CHATROOM\n");
				
				// send the leave message to the rest of the chatroom
				while(ptr->next_node != NULL)
				{
					// go to the next chat node
					ptr = ptr->next_node;
					
					// if the selected chat node is not the leaving node
					if(ptr->chat_node->log_name != inputMessage->chat_node.log_name)
					{
						// write the message to the current chat node
						send_message_to_server(ptr->chat_node->thread_num, outputMessage);
					}
				}
				
				// DEBUG: CHECK IF PASSED SENDING THE MESSAGE TO ALL CLIENTS
				printf("PASSED SENDING THE MESSAGE TO ALL CLIENTS\n");
				
				// close the thread between the sender and server
				close(clientSocket);
				pthread_exit(EXIT_SUCCESS);
				
			// if the SHUTDOWN ALL identifier was sent
			case SHUTDOWN_ALL:
				// DEBUG: CHECK IF CORRECTLY GOT IDENTIFIER
				printf("CASE: SHUTDOWN ALL\n");
				
				// determine sender
				while(strcmp(ptr->chat_node->log_name, inputMessage->chat_node.log_name) != 0)
				{
					// go to the next chat node
					ptr = ptr->next_node;
				}
				
				// form the message struct so we can send a message to all the other clients before shutting down
				outputMessage = create_message(SHUTDOWN_ALL, inputMessage->note, ptr->chat_node);
				
				// reset the pointer to the head.
				ptr = args->chatroomList;
				
				// send the leave message to the entire chatroom
				while(ptr->next_node != NULL)
				{
					// go to the next chat node
					ptr = ptr->next_node;
					
					// write the message to the current chat node
					send_message_to_server(ptr->chat_node->thread_num, outputMessage);
				}
				
				// shutdown the server
				exit(EXIT_SUCCESS);
				
			// if the NOTE identifier was sent
			case NOTE:
				// DEBUG: CHECK IF CORRECTLY GOT IDENTIFIER
				printf("CASE: NOTE\n");
				
				// move past the null head
				ptr = ptr->next_node;
				
				// determine sender
				while(strcmp(ptr->chat_node->log_name, inputMessage->chat_node.log_name) != 0)
				{
					// go to the next chat node
					ptr = ptr->next_node;
				}
				
				// form the message struct so we can send the note to all the other clients before shutting down
				outputMessage = create_message(NOTE, inputMessage->note, ptr->chat_node);

				// reset the pointer to the head.
				ptr = args->chatroomList;

				// send the message to everyone but the sender
				while(ptr->next_node != NULL)
				{
					// move to the next node
					ptr = ptr->next_node;
					
					// make sure the current node isn't the sender
					if(strcmp(ptr->chat_node->log_name, inputMessage->chat_node.log_name) != 0)
					{
						// write the message to the current chat node
						send_message_to_server(ptr->chat_node->thread_num, outputMessage);
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
