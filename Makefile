PROGRAMS = sneakyMessage
CC = gcc
CFLAGS = -Wall -ggdb
#CFLAGS = -Wall -ggdb -std=c99 - IF ON FOX SERVERS, USE THIS OPTION
all: $(PROGRAMS)

clean:
	rm -f *.o

bmpInterface.o: bmpInterface.c bmpInterface.h 
	$(CC) $(CFLAGS) -c bmpInterface.c
decoder.o: decoder.c decoder.h bmpInterface.h
	$(CC) $(CFLAGS) -c decoder.c
encoder.o: encoder.c encoder.h bmpInterface.h
	$(CC) $(CFLAGS) -c encoder.c
sneakyMessage.o: sneakyMessage.c bmpInterface.h encoder.h decoder.h
	$(CC) $(CFLAGS) -c sneakyMessage.c
sneakyMessage: sneakyMessage.o encoder.o bmpInterface.o decoder.o
	$(CC) $(CFLAGS) -o sneakyMessage sneakyMessage.o encoder.o decoder.o bmpInterface.o
