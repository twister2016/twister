#!/bin/bash
while :
do
 if pidof -x "UDP_SEND" >/dev/null;
 then
   echo "Running"
   break
 else
   echo "Not Running"
   screen -d -m ./UDP_SEND
   sleep 1
 fi
done
