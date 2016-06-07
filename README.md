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
make
sudo make install
```
### Configure Twister and ports configurations

Edit  twister configurations in `/home/twister/config/twister.conf` file. 

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
Edit port configurations in `/home/twister/config/config.json`. Provide the network configurations for the NIC to be used as twister port.
For example, final configuration in `/home/twister/config/config.json` file may look like below:
```
[{"ip_addrs": [{ "ip_address": "10.0.2.129", "gateway_ip": "10.10.2.1", "subnet_mask": "255.255.255.0"}], "port_name": "tw0"}]
```
Run `twister-config`  to apply the changes.
```
twister-config
```
This script will apply the configurations defined in `/home/twister/config/twister.conf` and generate a json file `/home/twister/config/twister_api.json` which will be used by Twister API.

## Applications
- [Ethernet Echo Application](https://github.com/twister2016/twister/blob/master/documentation/TW_ETHERNET.md) This application performs Layer-2 forwarding for each packet that is received on the port. All the other fields in the packets remain the same - only the MAC addresses are affected.
- [Gateway Application](https://github.com/twister2016/twister/blob/master/documentation/TW_GATEWAY.md) This application plays the role of layer-3 router at smaller scale. The purpose is to harness the abstraction and functionality of Twister API to write sophisticated networking applications. This router forwards the packets received from one network to another using layer-3 forwarding. It also responds to the ARP requests as well as PING request on its both interfaces.
- [Ping](https://github.com/twister2016/twister/blob/master/documentation/TW_PING.md) This application sends the ICMP packets (ping), it takes IP from console, and sends the ICMP packets to this machine. When it gets the ICMP reply packet, it displays the ICMP parameters to user, like TTL, RTT, sequence number etc.
- [UDP Echo](https://github.com/twister2016/twister/blob/master/documentation/TW_UDPECHO.md) This application  is an example of packet processing built on Twister API to echo back the UDP traffic.
- [ICMP Echo](https://github.com/twister2016/twister/blob/master/documentation/TW_ICMPECHO.md) This application responds for the ICMP(Ping) packets.
- [Reverse ARP Application](https://github.com/twister2016/twister/blob/master/documentation/TW_REVERSE.md) This application implements the reverse address resolution protocol. It sends the RARP request and displays the IP resolved from RARP response.
- [IPERF Application](https://github.com/twister2016/twister/blob/master/documentation/TW_IPERF.md) This application provides functioanlity to measure maximum achievable bandwidth on IP conncection.




