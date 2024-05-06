#include "chat_node.h"

void add_chat_node(ChatNodeLL* chatNodeLL, ChatNode* insertionNode) {
    ChatNodeLL* tempNode = chatNodeLL;
    if (chatNodeLL == NULL)
    {
        chatNodeLL = insertionNode;
        return;
    }
    // Iterate through each chat node in linked list until the next node is NULL
    while(tempNode->nextNode != NULL) tempNode = tempNode->nextNode;

    // Set the next node pointer equal to the insertion node
    tempNode->nextNode = (ChatNodeLL*) malloc(sizeof(ChatNodeLL));
    tempNode = tempNode->nextNode;
    tempNode->chatNode = insertionNode;

}

ChatNode* create_chat_node(char* ip, int port_num, char* log_name) {
    // Allocate memory for chat node struct
    ChatNode* newNode = (ChatNode*) malloc(sizeof(ChatNode));

    // Assign parameter values to newly created chat node struct
    strcpy(newNode->ip, ip);
    newNode->port_num = port_num;
    strcpy(newNode->log_name, log_name);

    // Return struct
    return newNode;
}

ChatNodeBounds* create_chat_node_bounds() {
    // Allocate memory for chat node bounds struct
    ChatNodeBounds* newNode_bounds = (ChatNodeBounds*)malloc(sizeof(ChatNodeBounds));

    newNode_bounds->firstNode = NULL;
    newNode_bounds->lastNode = NULL;

    // Return pointer to newly allocated memory
    return newNode_bounds;
}

void remove_chat_node(ChatNodeLL* chatNodeLL, ChatNode* deletionNode) {
    
    ChatNodeLL* afterNode, *freeNode;
    ChatNodeLL* tempNode = chatNodeLL;
    ChatNodeLL* nextNode = tempNode->nextNode;

 
    // Iterate through each chat node in linked list until the next node is the deletion node
    while (nextNode->chatNode != deletionNode) 
    {
        tempNode = nextNode;
        nextNode = tempNode->nextNode;

        // Assume deletion node does not exist
        if (nextNode == NULL) 
        {
        return;
        }
    }
    // Store pointer to next node in a variable
    afterNode = nextNode->nextNode;

    // Set next node pointer equal to the deletion node's next node pointer
    tempNode->nextNode = afterNode;

    free(nextNode);


    // Free deletion node
    free(deletionNode);
}
