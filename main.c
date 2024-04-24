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
	int serverSocket;			// descriptor of server socket
	int clientSocket;			// descriptor of the client socket
	struct sockaddr_in serverAddress; // for naming the server's listening socket
	char* properties_file = "PROPERTIES.properties";
	Properties* properties;
	pthread_t thread;
	int yes = 1;
	
	// create chat node list
	ChatNodeLL *chatroomList;
	
	// create chat node bounds
	ChatNodeBounds* bounds = create_chat_node_bounds();

	// read properties
	properties = property_read_properties(properties_file);

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
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr(SERVER_ADDR);
	serverAddress.sin_port = htons(PORT);

	// binding unnamed socket to a particular port
	if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) != 0) 
	{
		perror("Error binding socket");
		exit(EXIT_FAILURE);
	}

	// listen on the socket
	if (listen(serverSocket, 10) != 0)
	{
		perror("Error listening on socket");
		exit(EXIT_FAILURE);
	}
	
	// have the server print that it's ready
	printf("Ready for Connections\n");

	// infinite loop for server
	while(true)
	{
		clientSocket = accept(serverSocket, NULL, NULL);
		printf("\nServer with PID %d: accepted client\n", getpid());

		// handle the client
		pthread_create(&thread, NULL, talk_to_client, (void*)&clientSocket);
	}
	
	return EXIT_SUCCESS;
}
