#include "message.h"

Message* create_message(char messageType, char note[], ChatNode* chatNode) {
    // Allocate memory for message struct
    Message* newMessage = (Message*) malloc(sizeof(Message));

    // Assign parameter values to newly created message struct
    newMessage->messageType = messageType;
    for(int i = 0; i < 64; i++)
        newMessage->note[i] = note[i];
    newMessage->chatNode = *chatNode;

    // Return message struct
    return newMessage;
}

void send_message_to_server(int socket, Message* message) {
    ChatNode* chatNode = &message->chatNode;
    // Send message type to socket
    write(socket, &message->messageType, sizeof(char));

    // Send message note to socket
    write(socket, message->note, sizeof(char) * 64);

    // Send message chat node to socket
    write(socket, chatNode->ip, sizeof(char) * 15);
    int portNum = htonl(chatNode->portNum);
    write(socket, &portNum, sizeof(int));
    write(socket, chatNode->logName, sizeof(char) * 16);
}

void receive_message_from_server(int socket, Message* message) {
    int loopIndex;
    int portNum;
    ChatNode* chatNode = &message->chatNode;

    // Receive message type to socket
    read(socket, &message->messageType, sizeof(char));
    // Receive message note to socket
    for(loopIndex = 0; loopIndex < 64; loopIndex++)
        read(socket, message->note + loopIndex, sizeof(char));
       
    // Receive message chat node to socket
    for(loopIndex = 0; loopIndex < 15; loopIndex++)
        read(socket, chatNode->ip + loopIndex, sizeof(char));

    read(socket, &portNum, sizeof(int));
    chatNode->portNum = ntohl(portNum);

    for (loopIndex = 0; loopIndex < 16; loopIndex++)
        read(socket, chatNode->logName + loopIndex, sizeof(char));
}
