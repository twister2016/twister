#!/bin/bash

while :

do

 if pidof -x "ARP" >/dev/null;

 then

   pkill -9 ARP

   break

 else

   echo "Not Running"

   sudo ./ARP

   sleep 1

 fi

done
