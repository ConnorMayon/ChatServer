// header files
#include "client_handler.h"

// constants

// functions
// function to talk to a client
void talk_to_client(void* arg)
{
	// initialize varaibles
	char buff_out[2048]
	char input[2048];
	char identifier;
	
	// while the client is connected
	while(true)
	{
		// read the message sent from the client
		read(client_socket, &input, sizeof(input));
		identifier = input[0];
		
		// determine what identifier was sent in the message and start switch statement
		switch(identifier)
		{
			// if the JOIN identifier was sent
			case JOIN:
			
				// send the join message to the chatroom for the requesting client
				sprintf(buff_out, "%s has joined\n", name);
				write(clientSocket, &buff_out, sizeof(buff_out));
				
				break;				
				
			// if the LEAVE identifier was sent
			case LEAVE:
			
				// disconect the requesting client
				sprintf(buff_out, "L\n", name);
				write(clientSocket, &buff_out, sizeof(buff_out));
				
				// send the leave message to the chatroom for the requesting client
				sprintf(buff_out, "%s has left\n", name);
				write(clientSocket, &buff_out, sizeof(buff_out));
				
				break;
				
			// if the SHUTDOWN ALL identifier was sent
			case SHUTDOWN_ALL:
			
				// send all connected clients the shutdown code
				sprintf(buff_out, "L\n", name);
				write(clientSocket, &buff_out, sizeof(buff_out))
				
				// shutdown the server
				exit();
				
				break;
				
			// if the NOTE identifier was sent
			case NOTE:
			
				// send the message to the chatroom
				write(clientSocket, &input, sizeof(buff_out));
			
		// end of the switch statement
		}
	}
		
	//return NULL;
// end of function
}
