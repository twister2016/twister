#!/bin/bash

while :

do

 if pidof -x "ICMP_ECHO" >/dev/null;

 then

   sudo pkill -9 ICMP_ECHO
   sudo ./ICMP_ECHO
   break

 else

   echo "Not Running"

   sudo ./ICMP_ECHO

   sleep 1

 fi

done
