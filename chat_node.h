#ifndef CHAT_NODE_H
#define CHAT_NODE_H

typedef struct chatNodeStruct {
    char ip[15];
    int port_num;
    char log_name[16];
} ChatNode;

typedef struct chatNodeLL {
    ChatNode* chat_node;
    ChatNode* next_node;
} ChatNodeLL;

typedef struct chatNodeBounds {
    ChatNode* first_node;
    ChatNode* last_node;
} ChatNodeBounds;


void add_chat_node(ChatNodeLL* chat_node_ll, ChatNode* insertion_node);
ChatNode* create_chat_node(char* ip, int port_num, char* log_name);
ChatNode* create_chat_node_ll();
void remove_chat_node(ChatNodeLL* chat_node_ll, ChatNode* deletion_node);
#endif