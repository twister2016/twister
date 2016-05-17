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
	make install
```

## Applications
- [ARP Application](https://github.com/twister2016/twister/tree/master/examples/arp) This application implements the address resolution protocol, it takes input as IP address and finds the MAC address of that machine if it exist on same network.
- [Ethernet Echo Application](https://github.com/twister2016/twister/tree/master/examples/example_ethernet_echo) This application performs Layer-2 forwarding for each packet that is received on the port. All the other fields in the packets remain the same - only the MAC addresses are affected.
- [Gateway Application](https://github.com/twister2016/twister/tree/master/examples/example_gateway) This application plays the role of layer-3 router at smaller scale. The purpose is to harness the abstraction and functionality of Twister API to write sophisticated networking applications. This router forwards the packets received from one network to another using layer-3 forwarding. It also responds to the ARP requests as well as PING request on its both interfaces.
- [Ping](https://github.com/twister2016/twister/tree/master/examples/example_icmp_echo) This application sends the ICMP packets (ping), it takes IP from console, and sends the ICMP packets to this machine. When it gets the ICMP reply packet, it displays the ICMP parameters to user, like TTL, RTT, sequence number etc.
- [UDP Echo](https://github.com/twister2016/twister/tree/master/examples/example_udp_echo) This application  is an example of packet processing built on Twister API to echo back the UDP traffic.
- [ICMP Echo](https://github.com/twister2016/twister/tree/master/examples/icmp_reply) This application responds for the ICMP(Ping) packets.
- [Multi-Ethernet Echo](https://github.com/twister2016/twister/tree/master/examples/multiport_echo) This applications is an example of Layer-2 packet processing built on Twister API which also takes advantage of more than one ethernet port.
- [Reverse Application](https://github.com/twister2016/twister/tree/master/examples/reverseArp) This application implements the reverse address resolution protocol. It sends the RARP request and displays the IP resolved from RARP response.

### How to Compile:
To compile any applications, from the root directory of the Twister repository execute the following commands:
```
cd examples/<Application Name>
make clean
make
```
## License




