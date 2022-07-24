# This will be... you guessed, it the Makefile
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