#include "chat_node.h"

void add_chat_node(ChatNodeLL* chat_node_ll, ChatNode* insertion_node) {
    ChatNodeLL* temp_node = chat_node_ll;
    if (chat_node_ll == NULL)
    {
        printf("First node is null\n");
        chat_node_ll = insertion_node;
        return;
    }
    // Iterate through each chat node in linked list until the next node is NULL
    while(temp_node->next_node != NULL) temp_node = temp_node->next_node;

    // Set the next node pointer equal to the insertion node
    temp_node->next_node = (ChatNodeLL*) malloc(sizeof(ChatNodeLL));
    temp_node = temp_node->next_node;
    temp_node->chat_node = insertion_node;
    printf("\n\nTemp node: %p\n", temp_node->chat_node);
}

ChatNode* create_chat_node(char* ip, int port_num, char* log_name) {
    // Allocate memory for chat node struct
    ChatNode* new_node = (ChatNode*) malloc(sizeof(ChatNode));

    // Assign parameter values to newly created chat node struct
    strcpy(new_node->ip, ip);
    new_node->port_num = port_num;
    strcpy(new_node->log_name, log_name);

    // Return struct
    return new_node;
}

ChatNodeBounds* create_chat_node_bounds() {
    // Allocate memory for chat node bounds struct
    ChatNodeBounds* new_node_bounds = (ChatNodeBounds*)malloc(sizeof(ChatNodeBounds));

    new_node_bounds->first_node = NULL;
    new_node_bounds->last_node = NULL;

    // Return pointer to newly allocated memory
    return new_node_bounds;
}

void remove_chat_node(ChatNodeLL* chat_node_ll, ChatNode* deletion_node) {
    printf("\nRemoving node\n");
    ChatNodeLL* after_node, *free_node;
    ChatNodeLL* temp_node = chat_node_ll;
    ChatNodeLL* next_node = temp_node->next_node;

    printf("deletion node %p\n", deletion_node);

    printf("Entering loop\n");
    // Iterate through each chat node in linked list until the next node is the deletion node
    while (next_node->chat_node != deletion_node) 
    {
        printf("Temp node %p\n", temp_node);
        printf("Next node %p\n", next_node);
        temp_node = next_node;
        next_node = temp_node->next_node;

        // Assume deletion node does not exist
        if (next_node == NULL) 
        {
        printf("Deletion node not found\n");
        return;
        }
    }
    printf("Deletion node found\n");
    // Store pointer to next node in a variable
    after_node = next_node->next_node;

    // Set next node pointer equal to the deletion node's next node pointer
    temp_node->next_node = after_node;

    printf("next node: %p \n", next_node);
    printf("\ndeletion node: %p\n", deletion_node);
    free(next_node);

    printf("Freed temp node \n");

    // Free deletion node
    free(deletion_node);
    printf("\nFreed deletion node\n");
}

