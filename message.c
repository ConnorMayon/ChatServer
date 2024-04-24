#include "message.h"

Message* create_message(char message_type, char note[], ChatNode* chat_node) {
    // Allocate memory for message struct
    Message* new_message = (Message*) malloc(sizeof(Message));

    // Assign parameter values to newly created message struct
    new_message->message_type = message_type;
    for(int i = 0; i < 64; i++)
        new_message->note[i] = note[i];
    new_message->chat_node = *chat_node;

    // Return message struct
    return new_message;
}

void send_message_to_server(int socket, Message* message) {
    ChatNode* chat_node = &message->chat_node;
    // Send message type to socket
    write(socket, &message->message_type, sizeof(char));

    // Send message note to socket
    write(socket, message->note, sizeof(char) * 64);

    // Send message chat node to socket
    write(socket, chat_node->ip, sizeof(char) * 15);
    int port_num = htonl(chat_node->port_num);
    write(socket, &port_num, sizeof(int));
    write(socket, chat_node->log_name, sizeof(char) * 16);
}

void receive_message_from_server(int socket, Message* message) {
    int loop_index;
    int port_num;
    ChatNode* chat_node = &message->chat_node;

    // Receive message type to socket
    read(socket, &message->message_type, sizeof(char));
    // Receive message note to socket
    for(loop_index = 0; loop_index < 64; loop_index++)
        read(socket, message->note + loop_index, sizeof(char));
       
    // Receive message chat node to socket
    for(loop_index = 0; loop_index < 15; loop_index++)
        read(socket, chat_node->ip + loop_index, sizeof(char));

    read(socket, &port_num, sizeof(int));
    chat_node->port_num = ntohl(port_num);

    for (loop_index = 0; loop_index < 16; loop_index++)
        read(socket, chat_node->log_name + loop_index, sizeof(char));
}