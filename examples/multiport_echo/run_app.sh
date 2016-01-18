#!/bin/bash

while :

do

 if pidof -x "MULTI_ECHO" >/dev/null;

 then

   sudo pkill -9 MULTI_ECHO
   sudo ./MULTI_ECHO
   break

 else

   echo "Not Running"

   sudo ./MULTI_ECHO

   sleep 1

 fi

done
