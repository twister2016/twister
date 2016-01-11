#!/bin/bash

while :

do

 if pidof -x "ICMP_REPLY" >/dev/null;

 then

   pkill -9 ICMP_REPLY

   break

 else

   echo "Not Running"

   sudo ./ICMP_REPLY

   sleep 1

 fi

done
