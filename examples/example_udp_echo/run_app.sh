#!/bin/bash

while :

do

 if pidof -x "UDP_ECHO" >/dev/null;

 then

   sudo pkill -9 UDP_ECHO
   sudo ./UDP_ECHO
   break

 else

   echo "Not Running"

   sudo ./UDP_ECHO

   sleep 1

 fi

done
