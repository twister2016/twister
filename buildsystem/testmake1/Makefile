#Test Makefile
CC = gcc
FLAGS = -c
CCFLAGS = -o
SOURCES=test.c
OBJECTS = ${SOURCES:.c=.o}
BINARIES = output

all: ${OBJECTS}
	${CC} ${OBJECTS} ${CCFLAGS} ${BINARIES} 
clean:
	rm *.o *.c $(BINARIES)

.c.o:
	${CC} ${FLAGS} $< ${CCFLAGS} $@ 
