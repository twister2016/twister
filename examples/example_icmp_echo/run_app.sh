#!/bin/bash

while :

do

 if pidof -x "ICMP_ECHO" >/dev/null;

 then

   pkill -9 ICMP_ECHO

   break

 else

   echo "Not Running"

   sudo ./ICMP_ECHO

   sleep 1

 fi

done
