#!/bin/bash
if pidof -x "udp_echo_app" >/dev/null;
then
    echo "Process already running"
else
    echo "Process not running, starting..."
    sleep 1
    screen -dm ./udp_echo_app -c 1 -n 4 -b 00:03.0 -- -p 1

fi
