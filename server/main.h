// precompiler directives

// libraries
#include "client_handler.h"
#include "chat_node.h"
#include "message.h"
#include "properties.h"
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

// preprocessor directives
#define SERVER_ADDR "142.11.199.119"
#define PORT 23658
#define THREAD_COUNT 2
