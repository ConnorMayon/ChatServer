#ifndef CHAT_NODE_H
#define CHAT_NODE_H
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>


typedef struct chatNodeStruct {
    char ip[15];
    int port_num;
    char log_name[16];
    int thread_num;
} ChatNode;

struct ChatNodeLL {
    ChatNode* chat_node;
    struct ChatNodeLL* next_node;
};

struct ChatNodeLL;
typedef struct ChatNodeLL ChatNodeLL;

typedef struct chatNodeBounds {
    ChatNode* first_node;
    ChatNode* last_node;
} ChatNodeBounds;


void add_chat_node(ChatNodeLL* chat_node_ll, ChatNode* insertion_node);
ChatNode* create_chat_node(char* ip, int port_num, char* log_name);
ChatNodeBounds* create_chat_node_bounds();
void remove_chat_node(ChatNodeLL* chat_node_ll, ChatNode* deletion_node);
#endif