// header files
#include "client_handler.h"

// constants

// functions
// function to talk to a client
void* talk_to_client(void *_args)
{
	// initialize varaibles
	char buffOut[64];
	char identifier;
	char message[64];
	char chatNodeip[15];
	int chatNodePort;
	char chatNodeName[16];
	int senderThread;
	Message* outputMessage;
	struct args* args = (struct args*) _args;

	// lock mutex
	pthread_mutex_unlock(&clients_mutex);
	
	// create a pointer for the chatroom list
	ChatNodeLL *ptr = args->chatroomList;

	if(args->clientSocket == NULL)
	{
		close(args->clientSocket);
		pthread_exit(EXIT_FAILURE);
	}
	
	
	// while the client is connected
	while(1)
	{
		// set the ptr to the (null) head
		ptr = args->chatroomList;
		
		// read the message sent from the client in the stages sent
			// read the identifier
		read(args->clientSocket, &identifier, sizeof(identifier));
		
			// read the message
		read(args->clientSocket, &message, sizeof(message));
		
			// read the sender ip
		read(args->clientSocket, &chatNodeip, sizeof(chatNodeip));
		
			// read the sender port
		read(args->clientSocket, &chatNodePort, sizeof(chatNodePort));
		
			// read the sender name
		read(args->clientSocket, &chatNodeName, sizeof(chatNodeName));

		// DEBUG: PRINT THAT A NEW CASE HAS BEGAN
		printf("\n\n\n====== NEW CASE ======\n");
		
		// determine what identifier was sent in the message and start switch statement
		switch(identifier)
		{
			// if the JOIN identifier was sent
			case JOIN:
				// DEBUG: CHECK IF FINDING CASE AND MAKING OUTPUT
				printf("CASE: JOIN");
			
				// connect the client to the chatroom by adding them to the chat node.
					// create the new chat node to add later
				ChatNode* newNode = create_chat_node(chatNodeip, chatNodePort, chatNodeName);
				
					// add the new node's thread number manually
				newNode->thread_num = args->clientSocket;
				
					// add the new clinet to the chat node list
				add_chat_node(ptr, newNode);
				
					// set the new node as the first bounds if not already set
				if(args->bounds->first_node == NULL) args->bounds->first_node = newNode;
				
					// otherwise set the new node as the last bounds
				else args->bounds->last_node = newNode;

				// form the message struct so we can send a message to all the other clients
				outputMessage = create_message(JOIN, buffOut, newNode);
				
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
				
				// end of this case
				break;				
				
			// if the LEAVE identifier was sent
			case LEAVE:
			
				// DEBUG: CHECK IF FINDING CASE
				printf("CASE: LEAVE\n");

				// determine sender
				while(ptr->chat_node->log_name != chatNodeName)
				{
					// go to the next chat node
					ptr = ptr->next_node;
				}
				
				// form the message struct so we can send a message to all the other clients
				outputMessage = create_message(LEAVE, buffOut, ptr->chat_node);
				
				// reset the pointer to the head.
				ptr = args->chatroomList;
				
				// send the leave message to the rest of the chatroom
				while(ptr->next_node != NULL)
				{
					// go to the next chat node
					ptr = ptr->next_node;
					
					// if the selected chat node is not the leaving node
					if(ptr->chat_node->log_name != chatNodeName)
					{
						// write the message to the current chat node
						send_message_to_server(ptr->chat_node->thread_num, outputMessage);
					}
				}
				
				// DEBUG: CHECK IF PASSED SENDING THE MESSAGE TO ALL CLIENTS
				printf("PASSED SENDING THE MESSAGE TO ALL CLIENTS\n");
				
				// close the thread between the sender and server
				close(args->clientSocket);
				pthread_exit(EXIT_SUCCESS);
				
			// if the SHUTDOWN ALL identifier was sent
			case SHUTDOWN_ALL:
				// determine sender
				while(ptr->chat_node->log_name != chatNodeName)
				{
					// go to the next chat node
					ptr = ptr->next_node;
				}
			
				// form the message struct so we can send a message to all the other clients before shutting down
				outputMessage = create_message(SHUTDOWN_ALL, buffOut, ptr->chat_node);
				
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
				printf("IDENTIFIER READ: NOTE\n");
				// send the message to everyone but the sender
				while(ptr->next_node != NULL)
				{
					ptr = ptr->next_node;
					
					// make sure the current node isn't the sender
					if(ptr->chat_node->log_name != chatNodeName)
					{
						// DEBUG: CHECK IF WHILE LOOP IS BEING HIT
						printf("WRITING TO THREAD: %d\n", ptr->chat_node->thread_num);
						printf("MESSAGE: %s\n", message);
						printf("CHAT NODE NAME: %s\n", chatNodeName);
						
						// write the indentifier
						write(ptr->chat_node->thread_num, &identifier, sizeof(identifier));
						
						// write the message
						write(ptr->chat_node->thread_num, &message, sizeof(message));
						
						// write the sender
						write(ptr->chat_node->thread_num, &chatNodeName, sizeof(chatNodeName));
					}
				}
				// DEBUG: CHECK IF PASSED THE SENDING OF THE NOTE TO ALL CLIENTS
				printf("PASSED THE SENDING OF THE NOTE TO ALL CLIENTS\n");
				// end of this case
				
		// end of the switch statement
		}
	}
	
	//return NULL;
// end of function
}
