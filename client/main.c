#include "main.h"

/************************************************************************
 * MAIN
 ************************************************************************/

int main() {                   
    int client_socket;                  // client side socket
 
    // create an unnamed socket, and then name it
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // set conn_args
    struct conn_args *args = malloc (sizeof (struct conn_args));
    args->connection = client_socket;
    args->connected = false;
    args->active = true;
    
    // !!-- malloc space for chat node data --!! (function not created yet - in outline)

    // thread
    pthread_t thread[THREAD_COUNT];
    pthread_create(&thread[0], NULL, runSending, args);
    pthread_create(&thread[1], NULL, runReceiving, args);
    
    while(true)
        {
        if(!args->active)
            {
            //finish thread
            for(int i = 0; i < THREAD_COUNT; i++)
                {
                pthread_join(thread[i], NULL);
                }

            free(args);
            printf("\n");
            break;
            }
        }
    return EXIT_SUCCESS;
}