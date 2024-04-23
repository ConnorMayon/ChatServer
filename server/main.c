// header files
#include <stdio.h>
#include <stdlib.h>
#include "main.h"

/* ************************************************************************* */
/* MAIN                                                                      */
/* ************************************************************************* */
int main()
{
	// initialize varaibles
	int serverSocket;                 // descriptor of server socket
	struct sockaddr_in serverAddress; // for naming the server's listening socket
	int yes = 1;
	char* properties_file = "PROPERTIES.properties";
    Properties* properties;

	// ignore SIGPIPE, sent when client disconnected
	signal(SIGPIPE, SIG_IGN);

	// create unnamed network socket for server to listen on
	if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Error creating socket");
		exit(EXIT_FAILURE);
	}

	// lose the pesky "Address already in use" error message
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) 
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	// bind the socket
	serverAddress.sin_family      = AF_INET;           // accept IP addresses
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); // accept clients on any interface
	serverAddress.sin_port        = htons(PORT);       // port to listen on

	// binding unnamed socket to a particular port
	if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) != 0) 
	{
		perror("Error binding socket");
		exit(EXIT_FAILURE);
	}

	// listen on the socket
	if (listen(serverSocket, NUM_CLIENTS) != 0)
	{
		perror("Error listening on socket");
		exit(EXIT_FAILURE);
	}
	
	// have the server print that it's ready
	printf("Ready for Connections\n");

	// read properties
	Properties* properties = property_read_properties(properties_file);
	
	// server loop
	while (true)
	{
		// accept connection to client
		int clientSocket = accept(serverSocket, NULL, NULL);
		printf("\nServer with PID %d: accepted client\n", getpid());

		// create thread to handle the client's request
		// note that this is a naive approach, i.e. there are race conditions
		// for now this is okay, assuming low load
		pthread_t thread;
		if (pthread_create(&thread, NULL, handleClient, (void*)&clientSocket) != 0)
		{
			perror("Error creating thread");
			exit(EXIT_FAILURE);
		}

		// detach the thread so that we don't have to wait (join) with it to reclaim memory.
		// memory will be reclaimed when the thread finishes.
		if (pthread_detach(thread) != 0)
		{
			perror("Error detaching thread");
			exit(EXIT_FAILURE);
		}
	}
}
