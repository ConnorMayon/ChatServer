CC = gcc
DEBUG = -g
CFLAGS = -pthread -Wall -std=c99 -pedantic -c $(DEBUG)
LFLAGS = -pthread -Wall -std=c99 -pedantic $(DEBUG)

Server : main.o client_handler.o chat_node.o message.o properties.o
	  $(CC) $(LFLAGS) main.o client_handler.o chat_node.o message.o properties.o -o server

main.o : main.c main.h
	       $(CC) $(CFLAGS) main.c

client_handler.o : client_handler.c client_handler.h 
	$(CC) $(CFLAGS) client_handler.c 

chat_node.o : chat_node.c chat_node.h
	    $(CC) $(CFLAGS) ../chat_node.c

message.o : message.c message.h
	    $(CC) $(CFLAGS) ../message.c

properties.o: properties.c properties.h
		$(CC) $(CFLAGS) ../properties.c

clean:
	\rm *.o server
