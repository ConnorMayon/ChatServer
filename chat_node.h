#ifndef CHAT_NODE_H
#define CHAT_NODE_H
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>


typedef struct chatNodeStruct {
    char ip[15];
    int portNnum;
    char logName[16];
    int threadNum;
} ChatNode;

struct ChatNodeLL {
    ChatNode* chatNode;
    struct ChatNodeLL* nextNode;
};

struct ChatNodeLL;
typedef struct ChatNodeLL ChatNodeLL;

typedef struct chatNodeBounds {
    ChatNode* firstNode;
    ChatNode* lastNode;
} ChatNodeBounds;


void add_chat_node(ChatNodeLL* chatNodeLL, ChatNode* insertionNode);
ChatNode* create_chat_node(char* ip, int portNum, char* logName);
ChatNodeBounds* create_chat_node_bounds();
void remove_chat_node(ChatNodeLL* chatNodeLL, ChatNode* deletionNode);
#endif
