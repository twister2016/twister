
include twister.mk

CC=gcc
APP=Router_G
SRCS=test.c


hellomake: test.c
	$(CC) -g $(SRCS) $(TWFLAGS) -o $(APP)

.PHONY: clean

clean:
	rm -f $(APP)

