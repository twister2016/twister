# Twister ICMP Echo Application

The ICMP echo application is a example of packet processing built on Twister API which responds for the ICMP(Ping) packets.

## Overview
The ICMP echo application, on receiveing the icmp packets on the ports determined the  forwarding for each packet that is received on the ports. All the other fields in the packets remain the same - only the MAC addresses are affected as follows:

- The type field in ICMP packet is changed to make the packet as reply of an ICMP packet
- The checksum of the ICMP header is calculated
- The source IP address is replaced by the TX_PORT IP address
- The destination IP address is replaced by the source IP address of Querier
- The checksum of the Layer is calculated
- The source MAC address is replaced by the TX_PORT MAC address
- The destination MAC address is replaced by the Source MAC address of the received packet

##Create AWS instances for Twister

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
sudo twicmpecho
```
