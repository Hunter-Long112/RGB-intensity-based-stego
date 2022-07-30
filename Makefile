PROGRAMS = sneakyMessage
CC = gcc
CFLAGS = -ggdb
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

# I always need a reference when creating a Makefile, so here is one from my data structures class

# Makefile commentsÖÖ
# PROGRAMS = driver
# CC = gcc
# CFLAGS = -Wall -g
# all: $(PROGRAMS)
# clean:
# 	rm -f *.o
# # C compilations
# queue.o: queue.c queue.h process.h
# 	$(CC) $(CFLAGS) -c queue.c
# processSimulator.o: processSimulator.c processSimulator.h process.h
# 	$(CC) $(CFLAGS) -c processSimulator.c
# multilevelQueueScheduler.o: multilevelQueueScheduler.c queue.h processSimulator.h process.h
# 	$(CC) $(CFLAGS) -c multilevelQueueScheduler.c
# driver.o: driver.c multilevelQueueScheduler.h queue.h processSimulator.h process.h
# 	$(CC) $(CFLAGS) -c driver.c
# # Executable programs
# driver: driver.o queue.o processSimulator.o multilevelQueueScheduler.o
# 	$(CC) $(CFLAGS) -o driver driver.o queue.o processSimulator.o multilevelQueueScheduler.o
