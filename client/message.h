#include "chat_node.h"
#ifndef MESSAGE_H
#define MESSAGE_H

#define JOIN 'j'
#define LEAVE 'l'
#define SHUTDOWN_ALL 's'
#define NOTE 'n'

typedef struct messageStruct {
    char message_type;
    char note[64];
    ChatNode* chat_node;
} Message;


Message* create_message(char message_type, char note[], ChatNode* chat_node);
void send_message_to_server(int socket, Message* message);
void receive_message_from_server(int socket, Message* message);
#endif