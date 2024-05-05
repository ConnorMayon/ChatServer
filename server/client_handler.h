// precompiler directives
#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

// libraries
#include "../chat_node.h"
#include "../message.h"
#include "../properties.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

// socket/bind/listen/accept
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

// thread
#include <pthread.h>
#include <unistd.h> 

// read/write/close
#include <sys/uio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

// function declarations
void* talk_to_client(void *_args);

// mutex
extern pthread_mutex_t clients_mutex

// struct
struct args
{
	int clientSocket;
	ChatNodeLL* chatroomList;
	ChatNodeBounds* bounds;
};
#endif
