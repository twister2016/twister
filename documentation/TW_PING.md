# Twister ICMP Echo Application

This application sends the ICMP packets (ping), it takes ip from console, and sends the ICMP packets to this machine. When it gets the ICMP reply packet, it displays the ICMP parameters to user, like TTL, RTT, sequence number etc.

## Overview
ICMP Echo application construct the ICMP Echo packet by putting icmp_echo header on top of ipv4 header. This
- The source MAC address is replaced by the TX_PORT MAC address
- Destination MAC, if not known, is acquired by ARP.
- An ICMP packet is constructed with sequence number starting from 1 and incremented each time.
- On receiving ICMP reply, RTT is calculated and displayed on console.

## Create AWS instances for Twister

Please follow [this guide](/documentation/Twister_AWS.md) for creating AWS instances for Twister. This application requires only one twister instance but another instance on same network will be required to test the application.

Also ensure to connect one extra NIC for use by Twister Application. The VMs will automatically reboot one time to apply updates required for Twister.

## Configure Twister and ports configurations

Please follow [this guide](/README.md) for configuring Twister.

## Compiling and Running the Application
Go to twister repository folder and compile twister applications in `twister/applications` folders

```
cd twister/applications
sudo make install 
```

To run the application in linuxapp environment
```
sudo twping <ip address>
```
