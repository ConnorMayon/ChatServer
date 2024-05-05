// header files
#include "client_handler.h"

// constants

// functions
// function to talk to a client
void* talk_to_client(void *_args)
{
	// initialize varaibles
	char buffOut[2048];
	char identifier;
	char message[64];
	char chatNodeip[15];
	int chatNodePort;
	char chatNodeName[16];
	struct args* args = (struct args*) _args;

	// lock mutex
	pthread_mutex_unlock(&clients_mutex);
	
	// create a pointer for the chatroom list
	ChatNodeLL *ptr = args->chatroomList;
	
	
	// while the client is connected
	while(1)
	{
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
		
		// DEBUG CHECK IF READING
		printf("\n\n=========NEW CASE=========\n");
		printf("IDENTIFER: %i\n", identifier);
		printf("MESSAGE: %s\n", message);
		printf("CHATNODEIP: %s\n", chatNodeip);
		printf("CHATNODEPORT: %i\n", chatNodePort);
		printf("CHATNODENAME: %s\n", chatNodeName);
		
		// determine what identifier was sent in the message and start switch statement
		switch(identifier)
		{
			// if the JOIN identifier was sent
			case JOIN:
			
				// form the join message that will be sent to the chatroom
				sprintf(buffOut, "%s has joined\n", chatNodeName);
				
				// DEBUG: CHECK IF FINDING CASE AND MAKING OUTPUT
				printf("CASE: JOIN, JOIN MESSAGE TO BE SENT: %s\n", buffOut);
			
				// send the join message to the chatroom for the requesting client
				while(ptr->next_node != NULL)
				{
    					// go to the next chat node
					ptr = ptr->next_node;
     
					// write to the current chat node
					write(ptr->chat_node->thread_num, &buffOut, sizeof(buffOut));
				}
				
				// DEBUG: CHECK IF MAKES IT PASSED SENDING TO ALL CLIENTS
				printf("PASSED SENDING JOIN MESSAGE TO ALL CLIENTS\n");
			
				// connect the client to the chatroom by adding them to the chat node.
					// create the new chat node to add later
				ChatNode* newNode = create_chat_node(chatNodeip, chatNodePort, chatNodeName);
				
					// add the new node's thread number manually
				newNode->thread_num = args->clientSocket;

				// DEBUG: PRINT THE NEW CHAT NODE
				printf("IP: %s,  ", newNode->ip);
				printf("PORT: %d,  ", newNode->port_num);
				printf("NAME: %s,  ", newNode->log_name);
				printf("THREAD: %d\n", newNode->thread_num);
				
					// add the new clinet to the chat node list
				add_chat_node(ptr, newNode);
				
				
				printf("Chat node: %i\n\n", t_node->chat_node);
				
				
				// DEBUG: CHECK IF MAKES IT PASSED MAKING CHAT NODE
				printf("PASSED MAKING CHAT NODE\n");
				
					// set the new node as the first bounds if not already set
				if(args->bounds->first_node == NULL) args->bounds->first_node = newNode;
				
					// otherwise set the new node as the last bounds
				else args->bounds->last_node = newNode;

				// DEBUG: TEST THE LINKED LIST CHAT NODE
				printf("---TESTING LINKED LIST---\n");
				ptr = args->chatroomList;
				while(ptr->next_node != NULL)
				{
					// go to the next chat node
					ptr = ptr->next_node;
					
					printf("IP: %s,  ", ptr->chat_node->ip);
					printf("PORT: %d,  ", ptr->chat_node->port_num);
					printf("NAME: %s,  ", ptr->chat_node->log_name);
					printf("THREAD: %d\n", ptr->chat_node->thread_num);
				}
				
				// DEBUG: CHECK IF MAKES IT PASSED MAKING CHAT NODE
				printf("PASSED SETTING CHAT NODE BOUNDS\n");
				
				// end of this case
				break;				
				
			// if the LEAVE identifier was sent
			case LEAVE:
			
				// DEBUG: CHECK IF FINDING CASE
				printf("CASE: LEAVE\n");
			
				// disconect the requesting client
				identifier = LEAVE;
				write(args->clientSocket, &identifier, sizeof(identifier));
				
				// DEBUG: CHECK IF PASSED THE LEAVE MESSAGE WRITE COMMAND
				printf("PASSED THE LEAVE MESSAGE WRITE COMMAND\n");
				
				// create the leave message to sent to the chatroom
				identifier = NOTE;
				sprintf(buffOut, "%s has left\n", chatNodeName);
				
				// send the leave message to the entire chatroom
				while(ptr->next_node != NULL)
				{
					ptr = ptr->next_node;
					write(ptr->chat_node->thread_num, &identifier, sizeof(identifier));
					write(ptr->chat_node->thread_num, &buffOut, sizeof(buffOut));
				}
				
				// DEBUG: CHECK IF PASSED SENDING THE MESSAGE TO ALL CLIENTS
				printf("PASSED SENDING THE MESSAGE TO ALL CLIENTS\n");
				
				// drop the thread
				pthread_detach(pthread_self());
				
				// DEBUG: CHECK IF PASSED THE THREAD BEING DROPPED
				printf("PASSED THE THREAD BEING DROPPED\n");
				
				// end of this case
				break;
				
			// if the SHUTDOWN ALL identifier was sent
			case SHUTDOWN_ALL:
			
				// send all connected clients the shutdown code
				sprintf(buffOut, "L\n");
				while(ptr->next_node != NULL)
				{
					ptr = ptr->next_node;
					write(ptr->chat_node->thread_num, &buffOut, sizeof(buffOut));
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
					
					// DEBUG: CHECK IF WHILE LOOP IS BEING HIT
					printf("WHILE LOOP ENTERED\n");
					
					// make sure the current node isn't the sender
					if(ptr->chat_node->log_name != chatNodeName)
					{
						// DEBUG: CHECK IF WHILE LOOP IS BEING HIT
						printf("IF STATEMENT ENTERED\n");
						printf("WRITING TO THREAD: %d\n", ptr->chat_node->thread_num);
						printf("WRITING TO THREAD: %s\n", message);
						printf("WRITING TO THREAD: %s\n", chatNodeName);
						
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
