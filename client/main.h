// libraries (chat_node.h)
#include "sender_handler.h"
#include "receiver_handler.h"
#include "../chat_node.h"

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


// preprocessor directives
#define THREAD_COUNT 2

