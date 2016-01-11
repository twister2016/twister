#!/bin/bash

while :

do

 if pidof -x "ARP" >/dev/null;

 then

   sudo pkill -9 ARP
   sudo ./ARP
   break

 else

   echo "Not Running"

   sudo ./ARP

   sleep 1

 fi

done
