#!/bin/bash
pkill udp_send_app
./udp_send_app -c 1 -n 4 -b 00:03.0 -- -p 1
