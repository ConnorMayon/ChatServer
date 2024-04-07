#include "message.h"

Message* create_message(char message_type, char* note, ChatNode chat_node) {
    // Allocate memory for message struct

    // Assign parameter values to newly created message struct

    // Return message struct
}

void send_message_to_server(int socket, Message* message) {
    // Send message type to socket

    // Send message note to socket

    // Send message chat node to socket
}

void receive_message_from_server(int socket, Message* message) {
    // Receive message type to socket
       
    // Receive message note to socket
       
    // Receive message chat node to socket
}