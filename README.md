## Twister API

## Build Status:
[![Build Status](https://travis-ci.org/twister2016/twister.png)](https://travis-ci.org/twister2016/twister)

## How To:
### Create AWS instances for Twister

To create required AWS instances for twister please follow [this guide](/documentation/Twister_AWS.md).

Also ensure to connect one extra NIC for use by Twister Application. The VMs will automatically reboot one time to apply updates required for Twister.

### Clone
The Twister source can be clone using the following command:

```
  git clone https://github.com/twister2016/twister
```

### Compile and Install
The Twister package can be installed using the following commands:

```
cd twister
sudo make
sudo make install
```
### Configure Twister and ports configurations

Edit  twister configurations in `/etc/twister/twister.conf` file. 

Append comma separated names of interfaces to be used by Linux in blacklist.
Similarly append comma separated names of interfaces to be used by Twister in whitelist.
Provide number of cores to be used by Twister. 

For example, final configuration file may look like below:
```
[DEFAULT]

blacklist=eth0
whitelist=eth1
cores=1
```
Edit port configurations in `/etc/twister/config.json`. Provide the network configurations for the NIC to be used as twister port.
For example, final configuration in `/etc/twister/config.json` file may look like below:
```
[{"ip_addrs": [{ "ip_address": "10.0.2.129", "gateway_ip": "10.0.2.1", "subnet_mask": "255.255.255.0"}], "port_name": "tw0"}]
```
Run `twister-config`  to apply the changes.
```
sudo twister-config
```
This script will apply the configurations defined in `/etc/twister/twister.conf` and generate a json file `/etc/twister/twister_api.json` which will be used by Twister API.

## Applications
- [Ping](documentation/example_applications.md#twping) This application sends the ICMP packets (ping), it takes IP from console, and sends the ICMP packets to this machine. When it gets the ICMP reply packet, it displays the ICMP parameters to user, like TTL, RTT, sequence number etc.
- [ICMP Echo](documentation/example_applications.md#twicmpecho) This application responds for the ICMP(Ping) packets.
- [Reverse ARP Application](/documentation/example_applications.md#twrarp) This application implements the reverse address resolution protocol. It sends the RARP request and displays the IP resolved from RARP response.
- [iperf Application](documentation/example_applications.md#twiperf) This application provides functioanlity to measure maximum achievable bandwidth on IP conncection.




