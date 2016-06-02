## Twister API

## Build Status:
[![Build Status](https://travis-ci.org/twister2016/twister.png)](https://travis-ci.org/twister2016/twister)

## How To:

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
- [Ethernet Echo Application](https://github.com/twister2016/twister/blob/master/documentation/TW_ETHERNET.adoc) This application performs Layer-2 forwarding for each packet that is received on the port. All the other fields in the packets remain the same - only the MAC addresses are affected.
- [Gateway Application]() This application plays the role of layer-3 router at smaller scale. The purpose is to harness the abstraction and functionality of Twister API to write sophisticated networking applications. This router forwards the packets received from one network to another using layer-3 forwarding. It also responds to the ARP requests as well as PING request on its both interfaces.
- [Ping](https://github.com/twister2016/twister/blob/master/documentation/TW_PING.adoc) This application sends the ICMP packets (ping), it takes IP from console, and sends the ICMP packets to this machine. When it gets the ICMP reply packet, it displays the ICMP parameters to user, like TTL, RTT, sequence number etc.
- [UDP Echo](https://github.com/twister2016/twister/tree/master/examples/example_udp_echo) This application  is an example of packet processing built on Twister API to echo back the UDP traffic.
- [ICMP Echo](https://github.com/twister2016/twister/blob/master/documentation/TW_ICMPECHO.adoc) This application responds for the ICMP(Ping) packets.
- [Reverse ARP Application](https://github.com/twister2016/twister/blob/master/documentation/TW_REVERSE.adoc) This application implements the reverse address resolution protocol. It sends the RARP request and displays the IP resolved from RARP response.

### How to Compile:
To compile any applications, from the root directory of the Twister repository execute the following commands:
```
cd applications
sudo make clean
sudo make 
sudo make install
```




