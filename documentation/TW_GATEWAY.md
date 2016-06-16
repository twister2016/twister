#Twister Gateway example application 

This application plays the role of layer-3 router at smaller scale. The purpose is to harness the abstraction and functionality of Twister API to write sophisticated networking applications. This router forwards the packets received from one network to another using layer-3 forwarding. It also responds to the ARP requests as well as PING request on its both interfaces.

##Create AWS instances for Twister

Create 1 AWS instance for gateway and two more for testing the functionality. gateway instance should have 2 interfaces for twister. Please follow [this guide](/documentation/Twister_AWS.md) for creating AWS instances for Twister.

Also ensure to connect one extra NIC for use by Twister Application. The VMs will automatically reboot one time to apply updates required for Twister.

## Configure Twister and ports configurations

Please follow [this guide](/README.md) for configuring Twister.


## Run gateway (Twister Application)
Go to twister repository folder and compile twister applications in `twister/applications` folders

```
cd twister/applications
sudo make install 
```
Run gateway application in gateway instance (instance with two extra NICs),
```
sudo twgateway
```
