#!/bin/bash

while :

do

 if pidof -x "MULTI_ECHO" >/dev/null;

 then

   pkill -9 MULTI_ECHO

   break

 else

   echo "Not Running"

   sudo ./MULTI_ECHO

   sleep 1

 fi

done
