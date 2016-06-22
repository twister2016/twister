#Twister Example Applications



Twister applications can be run in three simple steps.


1. Create AWS EC2 VMs for Twister
2. Install and configure Twister
3. Run Twister Application


## 1. Create AWS EC2 VMs for Twister

Create two AWS EC2 VMs "twClient" and "twServer". Twister application can run in single application. Two VMs have been created for ease of demonstration. Please follow [this guide](/documentation/Twister_AWS.md) for creating AWS EC2 VMs for Twister.

Also ensure to connect one additional NIC for use by Twister Application. The VMs will automatically reboot one time to apply updates required for Twister.

## 2. Install and Configure Twister

Follow [How To](https://github.com/twister2016/twister#how-to) Section to clone, install and configure Twister.

## 3. Running Twister Application

Following sample applications have been built using Twister API.

1. twiperf
2. twping
3. twicmpecho
4. twarp
5. twrarp

### twiperf

`twiperf` is an iperf application built using Twister API. It currently supports sending UDP traffic and ethernet traffic (layer 2).

Run twiperf (server mode) in 'twServer' VM  with udp traffic.
```
sudo twiperf -u -s
```
Run twiperf (client mode) 'twClient' VM. Connect with IP address of twiperf server configured in 'twServer' VM.
```
sudo twiperf -u -c 10.0.2.129
```

Similarly twiperf can initiate a server with layer 2 traffic as well. Use `-e` or `--ethernet` flags to generate layer 2 traffic.

```
sudo twiperf -e -s
```

For more options checkout twiperf help with `-h` flag.

```
sudo twiperf -h
```
## twping
`twping` is a ping application built using Twister API. It works like linux `ping` application.

Run `twping` application as given below.
```
sudo twping <IPv4 address>
```

## twicmpecho
`twicmpecho` application responds back to `icmp` (`ping`) traffic. Run `twicmpecho` application to start icmp server.
```
sudo twicmpecho
```

Try sending ping traffic to IP address configured for the port in `/etc/twister/config.json`. `twicmpecho` application will respond back to the ping traffic.

## twarp
`twarp` application is an `ARP` application built using Twister API. `twarp` takes an IP address as input and returns its MAC address.

Run `twarp` application as given below.
```
sudo twarp <IP address>
```

## twrarp
`twrarp` application implements the reverse address resolution protocol. It sends the `RARP` request and displays the IP resolved from `RARP` response.

Run `RARP` application  with following command.
```
sudo twrarp
```
