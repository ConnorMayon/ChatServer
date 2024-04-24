CC = gcc
DEBUG = -g
CFLAGS = -pthread -Wall -std=c99 -pedantic -c $(DEBUG)
LFLAGS = -pthread -Wall -std=c99 -pedantic $(DEBUG)

Client : main_client.o receiver_handler.o sender_handler.o chat_node.o message.o
	  $(CC) $(LFLAGS) main_client.o receiver_handler.o sender_handler.o chat_node.o message.o -o client

Server : main_server.o client_handler.o chat_node.o message.o
	  $(CC) $(LFLAGS) main_server.o client_handler.o chat_node.o message.o -o server

main_client.o : main.c main.h
	       $(CC) $(CFLAGS) client/main.c

receiver_handler.o : receiver_handler.c receiver_handler.h 
	$(CC) $(CFLAGS) client/receiver_handler.c 

sender_handler.o : sender_handler.c sender_handler.h
	      $(CC) $(CFLAGS) client/sender_handler.c

chat_node.o : chat_node.c chat_node.h
	    $(CC) $(CFLAGS) chat_node.c

message.o : message.c message.h
	    $(CC) $(CFLAGS) message.c

main_server.o : main.c main.h
	       $(CC) $(CFLAGS) server/main.c

client_handler.o : client_handler.c client_handler.h 
	$(CC) $(CFLAGS) server/client_handler.c 

clean:
	\rm *.o client server
