include twister.mk

CC=gcc

DEB_DEPENDS  = make gcc

.PHONY: help build clean install

build: clean bootstrap ARP PING ICMPECHO ETHERECHO UDPECHO UDPSEND REVERSEARP MULTIETHERECHO GATEWAY

help:
	@echo "Make Targets:"
	@echo " clean       - wipe the build directory "
	@echo " install     - build and install all the applications"
	@echo " build       - build all the applications"

bootstrap:
ifeq ("$(shell lsb_release -si)", "Ubuntu")
	@MISSING=$$(apt-get install -y -qq -s $(DEB_DEPENDS) | grep "^Inst ") ; \
	if [ -n "$$MISSING" ] ; then \
	  echo "\nPlease install missing packages: \n$$MISSING\n" ; \
	  exit 1 ; \
	fi ; \
	exit 0
endif
	mkdir build
	

ARP: twarp.c
	$(CC) twarp.c $(TWFLAGS) -o build/twarp

PING: twping.c
	$(CC) twping.c $(TWFLAGS) -o build/twping

ICMPECHO: twicmpecho.c
	$(CC) twicmpecho.c $(TWFLAGS) -o build/twicmpecho

ETHERECHO: twetherecho.c
	$(CC) twetherecho.c $(TWFLAGS) -o build/twetherecho

UDPECHO: twudpecho.c
	$(CC) twudpecho.c $(TWFLAGS) -o build/twudpecho

UDPSEND: twudpsend.c
	$(CC) twudpsend.c $(TWFLAGS) -o build/twudpsend

REVERSEARP: twrarp.c
	$(CC) twrarp.c $(TWFLAGS) -o build/twrarp

MULTIETHERECHO: twmetherecho.c
	$(CC) twmetherecho.c $(TWFLAGS) -o build/twmetherecho

GATEWAY: twgateway.c
	$(CC) twgateway.c $(TWFLAGS) -o build/twgateway

install: build
	mkdir -p /home/twister/bin
	cp ./build/* /home/twister/bin/.
	export PATH="$(PATH):/home/twister/bin"
clean:
	rm -rf build

