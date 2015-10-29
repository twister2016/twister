#!/bin/bash
if pidof -x "UDP_SEND" >/dev/null;
then
    echo "Process already running"
else
    echo "Process not running, starting..."
    sleep 1
    screen -d -m ./UDP_SEND

fi
