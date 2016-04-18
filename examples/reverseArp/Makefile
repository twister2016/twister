include twister.mk

CC=gcc
APP=RARP
SRCS=rarp.c


hellomake: $(SRCS)
	$(CC) $(SRCS) $(TWFLAGS) -o $(APP)

.PHONY: clean

clean:
	rm -f $(APP)

