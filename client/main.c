#include "main.h"

/************************************************************************
 * MAIN
 ************************************************************************/
bool clientActive;
bool clientConnected;
int clientSocket;

int main(int argc, char *argv[]) {            
    char temp[50];
    int userPort;
    char userAdd[15];
    char userName[16];
 
    // create an unnamed socket, and then name it
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    // set conn_args
    struct conn_args *args = malloc (sizeof (struct conn_args));
    args->connected = false;
    args->active = true;

    clientActive = args->active;
    clientConnected = args->connected;

    // read properties
    char* paramFile = argv[1];

    FILE* file;
    file = fopen( paramFile, "r" );

    if(file != NULL)
        {
        fscanf(file, "%s %s %d", temp, temp, &args->serverPort);
        fscanf(file, "%s %s %s", temp, temp, args->serverAdd);
        fscanf(file, "%s %s %s", temp, temp, userName);
        fscanf(file, "%s %s %d", temp, temp, &userPort);
        fscanf(file, "%s %s %s", temp, temp, userAdd);
        
        printf("You are ready to go!\n");

        args->chatNode = create_chat_node(userAdd, userPort, userName);
        
        // thread
        pthread_t thread[THREAD_COUNT];
        if(pthread_create(&thread[0], NULL, runSending, args))
        {
            printf("Error creating sending thread");
        }

        if(pthread_create(&thread[1], NULL, runReceiving, args))
        {
            printf("Error creating receiving thread");        
        }


        //finish threads
        pthread_join(thread[0], NULL);
        pthread_join(thread[1], NULL);

	fclose(file);
        close(clientSocket);
        free(args);
        printf("\n");
        }
    else
    	{
    	printf("Cannot find properties file");
    	}
    return EXIT_SUCCESS;
}
