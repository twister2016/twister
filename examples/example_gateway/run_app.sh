#!/bin/bash

while :

do

 if pidof -x "Router_G" >/dev/null;

 then

   sudo pkill -9 Router_G
   sudo ./Router_G
   break

 else

   echo "Not Running"

   sudo ./Router_G

   sleep 1

 fi

done
