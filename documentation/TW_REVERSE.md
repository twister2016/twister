# Reverse ARP application


This application implements the reverse address resolution protocol. It sends the RARP request and displays the IP resolved from RARP response.

## Overview
This application implements the reverse RARP protocol.
. At any point user can type <exit> to exit from application.

- The source MAC address is replaced by the TX_PORT MAC address.
- Source IP is not defined i.e, zero
- Destination IP is not defined i.e. zero
- Destination MAC is written broadcast ff:ff:ff:ff:ff:ff
- The RARP server running in the network finds the packet and reply the sender with its ip.
- The returned ip with mac is shown on console.

##Create AWS instances for Twister

Please follow [this guide](/documentation/Twister_AWS.md) for creating AWS instances for Twister. This application requires only one twister instance.

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
sudo twrarp
```
