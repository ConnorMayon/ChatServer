// header files
#include "client_handler.h"

// constants

// functions
// function to talk to a client
void talk_to_client(void* arg)
{
	// initialize varaibles
	char buffOut[2048]
	char identifier;
	char message[64];
	char chatNodeip[15];
	int chatNodePort;
	char chatNodeName[16];
	
	
	// while the client is connected
	while(1)
	{
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
		
		// create a pointer for the chatroom list
		ChatNodeLL *ptr = chatroomList;
		
		// determine what identifier was sent in the message and start switch statement
		switch(identifier)
		{
			// if the JOIN identifier was sent
			case JOIN:
			
				// form the join message that will be sent to the chatroom
				sprintf(buffOut, "%s has joined\n", chatNodeName)
			
				// send the join message to the chatroom for the requesting client
				while(ptr->chat_node != NULL)
				{
					wrrite(clientSocket. &buffOut, sizeof(buffOut));
					ptr = ptr->next_node;
				}
			
				// connect the client to the chatroom by adding them to the chat node.
					// create the new chat node to add later
				ChatNode* newNode = create_chat_node(chatNodeip, chatNodePort, chatNodeName);
				
					// add the new clinet to the chat node list
				add_chat_node(chatroomList, newNode)
				clientCount++;
				
					// set the new node as the first bounds if not already set
				if(bounds->first_node == NULL) bounds->first_node = newNode;
				
					// otherwise set the new node as the last bounds
				else bounds->last_node = newNode;
				
				// end of this case
				break;				
				
			// if the LEAVE identifier was sent
			case LEAVE:
			
				// disconect the requesting client
				sprintf(identifier, "L");
				write(clientSocket, &identifier, sizeof(identifier));
				
				// send the leave message to the entire chatroom
				sprintf(identifier, "N");
				sprintf(buffOut, "%s has left\n", chatNodeName);
				while(ptr->chat_node != NULL)
				{
					write(clientSocket, &identifier, sizeof(identifier));
					wrrite(clientSocket, &buffOut, sizeof(buffOut));
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
					wrrite(clientSocket. &buffOut, sizeof(buffOut));
					ptr = ptr->next_node;
				}
				
				// shutdown the server
				exit();
				
			// if the NOTE identifier was sent
			case NOTE:
				
				// send the message to everyone but the sender
				while(ptr->chat_node-> != NULL)
				{
					// make sure the current node isn't the sender
					if(ptr->chat_node->log_name != chatNodeName)
					{
						// write the indentifier
						write(clientSocket, &identifier, sizeof(identifier));
						
						// write the message
						write(clientSocket, &message, sizeof(message));
						
						// write the sender
						write(clientSocket, &chatNodeName, sizeof(chatNodeName));
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

/* 
void *handle_client(void *arg){
	char buff_out[BUFFER_SZ];
	char name[32];
	int leave_flag = 0;

	cli_count++;
	client_t *cli = (client_t *)arg;

	// Name
	if(recv(cli->sockfd, name, 32, 0) <= 0 || strlen(name) <  2 || strlen(name) >= 32-1)
	{
		printf("Didn't enter the name.\n");
		leave_flag = 1;
	} 
	else
	{
		strcpy(cli->name, name);
		sprintf(buff_out, "%s has joined\n", cli->name);
		printf("%s", buff_out);
		send_message(buff_out, cli->uid);
	}

	bzero(buff_out, BUFFER_SZ);

	while(1)
	{
		if (leave_flag) 
		{
			break;
		}

		int receive = recv(cli->sockfd, buff_out, BUFFER_SZ, 0);
		if (receive > 0)
		{
			if(strlen(buff_out) > 0)
			{
				send_message(buff_out, cli->uid);

				str_trim_lf(buff_out, strlen(buff_out));
				printf("%s -> %s\n", buff_out, cli->name);
			}
		} 
		else if (receive == 0 || strcmp(buff_out, "exit") == 0)
		{
			sprintf(buff_out, "%s has left\n", cli->name);
			printf("%s", buff_out);
			send_message(buff_out, cli->uid);
			leave_flag = 1;
		} 
		else 
		{
			printf("ERROR: -1\n");
			leave_flag = 1;
		}
		bzero(buff_out, BUFFER_SZ);
	}

	//Delete client from queue and yield thread
	close(cli->sockfd);
	queue_remove(cli->uid);
	free(cli);
	cli_count--;
	pthread_detach(pthread_self());

	return NULL;
}
