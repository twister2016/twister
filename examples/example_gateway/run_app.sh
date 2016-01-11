#!/bin/bash

while :

do

 if pidof -x "Router_G" >/dev/null;

 then

   pkill -9 Router_G

   break

 else

   echo "Not Running"

   sudo ./Router_G

   sleep 1

 fi

done
