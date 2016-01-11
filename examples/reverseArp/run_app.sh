#!/bin/bash

while :

do

 if pidof -x "RARP" >/dev/null;

 then

   pkill -9 RARP

   break

 else

   echo "Not Running"

   sudo ./RARP

   sleep 1

 fi

done
