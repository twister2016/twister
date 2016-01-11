#!/bin/bash

while :

do

 if pidof -x "ETH_ECHO" >/dev/null;

 then

   pkill -9 ETH_ECHO

   break

 else

   echo "Not Running"

   sudo ./ETH_ECHO

   sleep 1

 fi

done
