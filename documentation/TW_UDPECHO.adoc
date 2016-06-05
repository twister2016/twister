# UDP Echo Example

The UDP example application is a example of packet processing built on Twister API to echo back the UDP traffic.

## Overview
The UDP example application performs Layer 4 forwarding for each packet that is received on the port. All the fields in the packets are affected as follows:

- The source Port is swapped by the destination Port
- The destination Port is swapped by the source Port
- The source IP address is replaced by the IP address of the port
- The destination IP address is replaced by the IP address of the sender
- The checksum is calculated for the IP Layer
- The source MAC address is replaced by the TX_PORT MAC address
- The destination MAC address is replaced by the Source MAC address of the received packet

the application also replies for the ARP queries.

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
sudo twudpecho
```
