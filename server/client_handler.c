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
	pthread_mutex_unlock(&clients_mutex);

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
		
		// DEBUG: PRINT THAT A NEW CASE HAS BEGAN
		//printf("\n\n\n====== NEW CASE ======\n");
		
		// DEBUG: MAKE SURE IT IS READING RIGHT
		//printf("---INPUT READING---\n");
		//printf("TYPE: %c, NOTE: %s, IP: %s, PORT: %i, NAME: %s.\n", inputMessage->message_type, inputMessage->note, inputMessage->chat_node.ip, inputMessage->chat_node.port_num, inputMessage->chat_node.log_name);
		
		// determine what identifier was sent in the message and start switch statement
		switch(inputMessage->message_type)
		{
			// if the JOIN identifier was sent
			case JOIN:
				// empty print statement because otherwise the first line of code is the newNode code and the compiler doesn't like that :(
				printf("");
				
				// connect the client to the chatroom by adding them to the chat node.
					// create the new chat node to add later
				ChatNode* newNode = create_chat_node(inputMessage->chat_node.ip, inputMessage->chat_node.port_num, inputMessage->chat_node.log_name);
				
					// add the new node's thread number manually
				newNode->thread_num = clientSocket;
				
					// add the new clinet to the chat node list
				add_chat_node(ptr, newNode);
				
				// DEBUG:
				//printf("Add chat node ptr %p\n", ptr);
				//printf("Add chat node ptr next node %p\n", ptr->next_node);
				//printf("Add chat node new node %p\n", newNode);
				
					// set the new node as the first bounds if not already set
				if(args->bounds->first_node == NULL) args->bounds->first_node = newNode;
				
					// otherwise set the new node as the last bounds
				else args->bounds->last_node = newNode;

				// form the message struct so we can send a message to all the other clients
				outputMessage = create_message(JOIN, inputMessage->note, newNode);

				// DEBUG: TEST THE OUTPUT MESSAGE
				//printf("---OUTPUT READING---\n");
				//printf("TYPE: %c, NOTE: %s, IP: %s, PORT: %i, NAME: %s.\n\n", outputMessage->message_type, outputMessage->note, outputMessage->chat_node.ip, outputMessage->chat_node.port_num, outputMessage->chat_node.log_name);
				
				// send the join message to the chatroom for the requesting client
				while(ptr->next_node != NULL)
				{
    					// go to the next chat node
					ptr = ptr->next_node;
					
					// if the selected chat node doesn't equal the newly created node
					if(ptr->chat_node != newNode)
					{
						// DEBUG: PRINT THREAD NUMBER
						//printf("THREAD NUMBER: %d.", ptr->chat_node->thread_num);
						
						// write the message to the current chat node
						send_message_to_server(ptr->chat_node->thread_num, outputMessage);
					}
				}
				// lock mutex
				//pthread_mutex_unlock(&clients_mutex);
				
				// end of this case
				break;				
				
			// if the LEAVE identifier was sent
			case LEAVE:
				// DEBUG: CHECK IF FINDING CASE
				//printf("CASE: LEAVE\n");
				
				// move past the null head
				ptr = ptr->next_node;

				// determine sender
				while(strcmp(ptr->chat_node->log_name, inputMessage->chat_node.log_name) != 0)
				{
					// go to the next chat node
					ptr = ptr->next_node;
				}
				
				// form the message struct so we can send a message to all the other clients
				outputMessage = create_message(LEAVE, inputMessage->note, &inputMessage->chat_node);

				// DEBUG: TEST WHO THE MESSAGE SENDER IS
				//printf("---OUTPUT READING---\n");
				//printf("TYPE: %c, NOTE: %s, IP: %s, PORT: %i, NAME: %s.\n\n", outputMessage->message_type, outputMessage->note, outputMessage->chat_node.ip, outputMessage->chat_node.port_num, outputMessage->chat_node.log_name);
				
				// reset the pointer to the head.
				ptr = headPtr;

				// DEBUG: START SENDING MESSAGE TO THE CHATROOM
				//printf("SENDING MESSAGE TO CHATROOM\n");
				
				// send the leave message to the rest of the chatroom
				while(ptr->next_node != NULL)
				{
					// go to the next chat node
					ptr = ptr->next_node;
					
					// if the selected chat node is not the leaving node
					if(strcmp(ptr->chat_node->log_name, inputMessage->chat_node.log_name) != 0 )
					{
						// DEBUG: TEST THE NAMES
						//printf("NAME FROM CHAT NODE: %s, NAME FROM ORIGINAL SENT MESSAGE: %s.\n", ptr->chat_node->log_name, inputMessage->chat_node.log_name);
						
						// DEBUG: TEST WHO THE MESSAGE SENDER IS
						//printf("---OUTPUT READING RIGHT BEFORE SEND---\n");
						//printf("TYPE: %c, NOTE: %s, IP: %s, PORT: %i, NAME: %s.\n\n", outputMessage->message_type, outputMessage->note, outputMessage->chat_node.ip, outputMessage->chat_node.port_num, outputMessage->chat_node.log_name);

						// write the message to the current chat node
						send_message_to_server(ptr->chat_node->thread_num, outputMessage);
					}
				}
				
				// DEBUG: CHECK IF PASSED SENDING THE MESSAGE TO ALL CLIENTS
				//printf("PASSED SENDING THE MESSAGE TO ALL CLIENTS\n");

				// remove the chat node
				remove_chat_node(headPtr, &ptr->chat_node);
				
				// close the thread between the sender and server
				close(clientSocket);
				pthread_exit(EXIT_SUCCESS);
				
			// if the SHUTDOWN ALL identifier was sent
			case SHUTDOWN_ALL:
				// DEBUG: CHECK IF CORRECTLY GOT IDENTIFIER
				//printf("CASE: SHUTDOWN ALL\n");
				
				// form the message struct so we can send a message to all the other clients before shutting down
				outputMessage = create_message(SHUTDOWN_ALL, inputMessage->note, &inputMessage->chat_node);
				
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
				//printf("CASE: NOTE\n");

				// form the message struct so we can send the note to all the other clients before shutting down
				outputMessage = create_message(NOTE, inputMessage->note, &inputMessage->chat_node);

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
