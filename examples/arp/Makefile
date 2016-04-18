
include twister.mk

CC=gcc
APP=ARP
SRCS=arp.c


hellomake: $(SRCS)
	$(CC) $(SRCS) $(TWFLAGS) -o $(APP)

.PHONY: clean

clean:
	rm -f $(APP)

