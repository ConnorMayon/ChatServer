CC = gcc
DEBUG = -g
CFLAGS = -pthread -Wall -std=c99 -pedantic -c $(DEBUG)
LFLAGS = -pthread -Wall -std=c99 -pedantic $(DEBUG)

Client : main.o receiver_handler.o sender_handler.o chat_node.o message.o
	  $(CC) $(LFLAGS) main.o receiver_handler.o sender_handler.o chat_node.o message.o -o client

main.o : main.c main.h
	       $(CC) $(CFLAGS) main.c

receiver_handler.o : receiver_handler.c receiver_handler.h 
	$(CC) $(CFLAGS) receiver_handler.c 

sender_handler.o : sender_handler.c sender_handler.h
	      $(CC) $(CFLAGS) sender_handler.c

chat_node.o : chat_node.c chat_node.h
	    $(CC) $(CFLAGS) ../chat_node.c

message.o : message.c message.h
	    $(CC) $(CFLAGS) ../message.c

clean:
	\rm *.o client
