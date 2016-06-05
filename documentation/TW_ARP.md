#ARP application

This application implements the address resolution protocol, it takes input as ipv4 address and finds the mac address of that machine if it exist on same network.

##Overview
This arp application construct the arp packet with broadcast mac and flood it into the same network. The machine whose MAC is required finds the pakcet and reply to sender. The sender extract the MAC and displays it to user. if machine does not reply within 5 seconds, ARP application assumes that machine does not exist.
. At any point user can type <exit> to exit from application.

- The source MAC address is replaced by the TX_PORT MAC address.
- Source IP is used from console input by user(ipv4).
- Destination MAC is written broadcast ff:ff:ff:ff:ff:ff
- The destined machine finds the packet and reply with its own mac address.
- The returned MAC address is shown on console.

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
sudo twarp <ip address>
```
