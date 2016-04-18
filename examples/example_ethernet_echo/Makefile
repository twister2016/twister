
include twister.mk

CC=gcc
APP=ETH_ECHO
SRCS=test.c


hellomake: test.c
	$(CC) $(SRCS) $(TWFLAGS) -o $(APP)

.PHONY: clean

clean:
	rm -f $(APP)

