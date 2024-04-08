// header files
#include "client_handler.h"

// constants

// functions

// function to talk to a client
// void talk_to_client(void* arg)

	// initialize varaibles
	
	// while the client is connected
		
		// read the message sent from the client
		
		// determine what identifier was sent in the message and start switch statement
		
			// if the JOIN identifier was sent
			
				// send the join message to the chatroom for the requesting client
				
			// if the LEAVE identifier was sent
			
				// disconect the requesting client
				
				// send the leave message to the chatroom for the requesting client
				
			// if the SHUTDOWN identifier was sent
			
				// send the client the shutdown code
				
				// disconect the requesting client
				
				// send the leave message to the chatroom for the requesting client
				
			// if the SHUTDOWN ALL identifier was sent
			
				// send all connected clients the shutdown code
				
				// disconnect all clients
				
				// shutdown the server
				
			// if the NOTE identifier was sent
			
				// send the message to the chatroom
				
			// if none of the following idenfiers were sent.
			
				// send an error message back to the client
				
				
		// end of the switch statement
		
	//return NULL;
// end of function
