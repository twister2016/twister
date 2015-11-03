#!/bin/bash
while :
do
 if pidof -x "UDP_ECHO" >/dev/null;
 then
   echo "Running"
   break
 else
   echo "Not Running"
   screen -d -m ./UDP_ECHO
   sleep 1
 fi
done

