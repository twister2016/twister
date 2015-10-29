#!/bin/bash
if pidof -x "UDP_ECHO" >/dev/null;
then
    echo "Process already running"
else
    echo "Process not running, starting..."
    sleep 1
    screen -d -m ./UDP_ECHO

fi
