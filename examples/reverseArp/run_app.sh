#!/bin/bash

while :

do

 if pidof -x "RARP" >/dev/null;

 then

   sudo pkill -9 RARP
   sudo ./RARP
   break

 else

   echo "Not Running"

   sudo ./RARP

   sleep 1

 fi

done
