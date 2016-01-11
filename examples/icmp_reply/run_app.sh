#!/bin/bash

while :

do

 if pidof -x "ICMP_REPLY" >/dev/null;

 then

   sudo pkill -9 ICMP_REPLY
   sudo ./ICMP_REPLY
   break

 else

   echo "Not Running"

   sudo ./ICMP_REPLY

   sleep 1

 fi

done
