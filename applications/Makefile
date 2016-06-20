include ../twister/mk/tw_sdk.mk

CC=gcc

DEB_DEPENDS  = make gcc

.PHONY: help build clean install

build: clean bootstrap ARP PING ICMPECHO REVERSEARP MULTIETHERECHO GATEWAY IPERF SMOKE

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

SMOKE: tw_env_smoke.c tw_eventloop_smoke.c
	$(CC) tw_env_smoke.c $(TWFLAGS) -o build/tw_env_smoke
	$(CC) tw_eventloop_smoke.c $(TWFLAGS) -o build/tw_eventloop_smoke

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

IPERF: twiperf.c
	$(CC) twiperf.c $(TWFLAGS) -o build/twiperf

install: 
	cp ./build/* /usr/bin/

clean:
	if [ -d "build" ] ; then \
	  rm -rf build ; \
	fi ; 

uninstall:  
	if [ -d "build" ] ; then \
  	  for i in $$( cd build && ls )  ; do  \
	    echo $$i ; \
	    rm -rf /usr/bin/$$i ; \
	  done ;  \
	  rm -rf build ; \
	fi ; 

