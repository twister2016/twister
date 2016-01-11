#!/bin/bash

while :

do

 if pidof -x "UDP_SEND" >/dev/null;

 then

   sudo pkill -9 UDP_SEND
   sudo ./UDP_SEND
   break

 else

   echo "Not Running"

   sudo ./UDP_SEND

   sleep 1

 fi

done
