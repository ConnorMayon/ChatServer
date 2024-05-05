#include "sender_handler.h"

// function that handles sending to client
void *runSending(void *_args)
    {
    struct conn_args *args = (struct conn_args *) _args;
    char input[1000];
    char blankString[64] = {'\0'};
    Message* sendMessage;

    while(true)
        {
        // continuously reads client input
        fgets(input, sizeof(input), stdin);
        printf("--input: %s", input);
        pthread_mutex_lock(&mutex);
        // translantes client input into control code
        if(strcmp(input, "LEAVE\n") == 0)
            {
            // if client said LEAVE
                if(clientConnected)
                {
                // if client is connected
                    // set code to L set message to empty
                    sendMessage = create_message(LEAVE, blankString, args->chatNode);
                    // send to server
                    send_message_to_server(clientSocket, sendMessage);

                    // end connection to client
                    clientConnected = false;
                    free(sendMessage);
                    close(clientSocket);
                }
                else
                {
                // if not connected
                    // display error
                printf("\nNOT CONNECTED TO SERVER: type JOIN to join chat and send messages and commands\n");
                }
            }
        else if(strcmp(input, "JOIN\n") == 0)
            {
            // if client said JOIN
                if(!clientConnected)
                {
                // if client is not connected
                    // set code to J set message to empty
                    sendMessage = create_message(JOIN, blankString, args->chatNode);

                    // connect to server
                    if(!makeConnection(args->serverAdd, args->serverPort)) exit(EXIT_FAILURE);
                    clientConnected = true;

                    // send to server
                    send_message_to_server(clientSocket, sendMessage);
                    
                    free(sendMessage);
                }
                else
                {
                // if client is connected
                    // display error
                printf("\nALREADY CONNECTED TO SERVER: type LEAVE to leave chat\n");
                }
            }
        else if(strcmp(input, "SHUTDOWN\n") == 0)
            {
            // if client said SHUTDOWN 
                if(clientConnected)
                {
                // if client is connected
                    // set code to L set message to empty
                    sendMessage = create_message(LEAVE, blankString, args->chatNode);
                    // send to server
                    send_message_to_server(clientSocket, sendMessage);

                    // end connection to client
                    clientConnected = false;
                    free(sendMessage);
                    close(clientSocket);
                }
                // handle shutdown
                exit(EXIT_SUCCESS);
            }
        else if(strcmp(input, "SHUTDOWN ALL\n") == 0)
            {
            // if client said SHUTDOWN ALL 
                if(clientConnected)
                {
                // if client is connected
                    // set code to S set message to empty
                    sendMessage = create_message(SHUTDOWN_ALL, blankString, args->chatNode);
                    // send to server
                    send_message_to_server(clientSocket, sendMessage);

                    free(sendMessage);
                }
                else
                {
                // if not connected
                    // display error
                printf("\nNOT CONNECTED TO SERVER: type JOIN to join chat and send messages and commands\n");
                }
            }
        else
            {
            // if client said anything else
                if(clientConnected)
                {
                // if client is connected
                    // set code to N set message to user input
                    sendMessage = create_message(NOTE, input, args->chatNode);
                    // send to server
                    send_message_to_server(clientSocket, sendMessage);

                    free(sendMessage);
                }
                else
                {
                // if not connected
                    // display error
                printf("\nCOMMAND NOT RECOGNIZED: type JOIN to join chat and send messages\n");
                }
            }


        pthread_mutex_unlock(&mutex);
        
        }
    return NULL;
    }

// connects to server
bool makeConnection(char *addr, int port)
    {
    struct sockaddr_in client_address;  

    // create addr struct
    client_address.sin_family = AF_INET;
    client_address.sin_addr.s_addr = inet_addr(addr);
    client_address.sin_port = htons(port);

    // connect to server socket
    if (connect(clientSocket, (struct sockaddr *)&client_address, sizeof(client_address)) == -1) 
        {
        perror("Error connecting to server!\n");
        return false;
        }
    return true;
    }
