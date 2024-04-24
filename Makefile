CC = gcc
DEBUG = -g
CFLAGS = -pthread -Wall -std=c99 -pedantic -c $(DEBUG)
LFLAGS = -pthread -Wall -std=c99 -pedantic $(DEBUG)

all: Client Server

Client : main_client.o client/receiver_handler.o client/sender_handler.o chat_node.o message.o
	  $(CC) $(LFLAGS) main_client.o client/receiver_handler.o client/sender_handler.o chat_node.o message.o -o client.exe

Server : main_server.o server/client_handler.o chat_node.o message.o
	  $(CC) $(LFLAGS) main_server.o server/client_handler.o chat_node.o message.o -o server.exe

main_client.o : client/main.c client/main.h
	       $(CC) $(CFLAGS) client/main.c -o main_client.o

receiver_handler.o : client/receiver_handler.c client/receiver_handler.h 
	$(CC) $(CFLAGS) client/receiver_handler.c 

sender_handler.o : client/sender_handler.c client/sender_handler.h
	      $(CC) $(CFLAGS) client/sender_handler.c

chat_node.o : chat_node.c chat_node.h
	    $(CC) $(CFLAGS) chat_node.c

message.o : message.c message.h
	    $(CC) $(CFLAGS) message.c

main_server.o : server/main.c server/main.h
	       $(CC) $(CFLAGS) server/main.c -o main_server.o

client_handler.o : server/client_handler.c server/client_handler.h 
	$(CC) $(CFLAGS) server/client_handler.c 

clean:
	\rm *.o client.exe server.exe main_client client/receiver_handler.o client/sender_handler.o
