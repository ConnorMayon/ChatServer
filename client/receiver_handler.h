#ifndef RECEIVER_HANDLER_H
#define RECEIVER_HANDLER_H

// libraries
#include "../message.h"

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

// read/write/close
#include <sys/uio.h>
#include <sys/types.h>
#include <unistd.h>

#include <signal.h>

//thread
#include <pthread.h>
#include <unistd.h> 

#include "../text_color.h"

//function prototype declarations
void *runReceiving();

struct conn_args {
       char serverAdd[15];
       int serverPort;
       ChatNode* chatNode;
     };


extern bool clientConnected;
extern int clientSocket;
extern pthread_mutex_t mutex;

#endif
