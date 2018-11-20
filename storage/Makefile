ALGS = $(wildcard algs/*.c)
TARGET = tmp
CC = gcc
HEADERS = header.h

default: disk

disk: clean
	$(CC) -o $(TARGET) disk_scheduling.c $(ALGS)

clean:
	-rm -f $(TARGET)
