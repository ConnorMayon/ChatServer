#include "chat_node.h"
#ifndef MESSAGE_H
#define MESSAGE_H

#define JOIN 'j'
#define LEAVE 'l'
#define SHUTDOWN_ALL 's'
#define NOTE 'n'

typedef struct messageStruct {
    char messageType;
    char note[64];
    ChatNode chatNode;
} Message;


Message* create_message(char messageType, char note[], ChatNode* chatNode);
void send_message_to_server(int socket, Message* message);
void receive_message_from_server(int socket, Message* message);
#endif
