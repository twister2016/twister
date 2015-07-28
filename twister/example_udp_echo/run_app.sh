#!/bin/bash
pkill udp_echo_app
./udp_echo_app -c 1 -n 4 -b 00:03.0 -- -p 1
