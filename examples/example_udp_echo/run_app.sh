#!/bin/bash
if pidof -x "UDP_ECHO" >/dev/null;
then
    echo "Process already running"
else
    echo "Process not running, starting..."
    sleep 1
    screen -dm ./UDP_ECHO

fi
