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
	
	
	// while the client is connected
	while(1)
	{
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
		
		// create a pointer for the chatroom list
		ChatNodeLL *ptr = args->chatroomList;
		
		// determine what identifier was sent in the message and start switch statement
		switch(identifier)
		{
			// if the JOIN identifier was sent
			case JOIN:
			
				// form the join message that will be sent to the chatroom
				sprintf(buffOut, "%s has joined\n", chatNodeName);
			
				// send the join message to the chatroom for the requesting client
				while(ptr->chat_node != NULL)
				{
					write(args->clientSocket, &buffOut, sizeof(buffOut));
					ptr = ptr->next_node;
				}
			
				// connect the client to the chatroom by adding them to the chat node.
					// create the new chat node to add later
				ChatNode* newNode = create_chat_node(chatNodeip, chatNodePort, chatNodeName);
				
					// add the new clinet to the chat node list
				add_chat_node(args->chatroomList, newNode);
				
					// set the new node as the first bounds if not already set
				if(args->bounds->first_node == NULL) args->bounds->first_node = newNode;
				
					// otherwise set the new node as the last bounds
				else args->bounds->last_node = newNode;
				
				// end of this case
				break;				
				
			// if the LEAVE identifier was sent
			case LEAVE:
			
				// disconect the requesting client
				identifier = LEAVE;
				write(args->clientSocket, &identifier, sizeof(identifier));
				
				// send the leave message to the entire chatroom
				identifier = NOTE;
				sprintf(buffOut, "%s has left\n", chatNodeName);
				while(ptr->chat_node != NULL)
				{
					write(args->clientSocket, &identifier, sizeof(identifier));
					write(args->clientSocket, &buffOut, sizeof(buffOut));
					ptr = ptr->next_node;
				}
				
				// end of this case
				break;
				
			// if the SHUTDOWN ALL identifier was sent
			case SHUTDOWN_ALL:
			
				// send all connected clients the shutdown code
				sprintf(buffOut, "L\n");
				while(ptr->chat_node != NULL)
				{
					write(args->clientSocket, &buffOut, sizeof(buffOut));
					ptr = ptr->next_node;
				}
				
				// shutdown the server
				exit(EXIT_SUCCESS);
				
			// if the NOTE identifier was sent
			case NOTE:
				
				// send the message to everyone but the sender
				while(ptr->chat_node != NULL)
				{
					// make sure the current node isn't the sender
					if(ptr->chat_node->log_name != chatNodeName)
					{
						// write the indentifier
						write(args->clientSocket, &identifier, sizeof(identifier));
						
						// write the message
						write(args->clientSocket, &message, sizeof(message));
						
						// write the sender
						write(args->clientSocket, &chatNodeName, sizeof(chatNodeName));
					}
					ptr = ptr->next_node;
				}
				// end of this case
				
		// end of the switch statement
		}
	}
	
	//return NULL;
// end of function
}
