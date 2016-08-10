## Twister API

## Build Status:
[![Build Status](https://travis-ci.org/twister2016/twister.png)](https://travis-ci.org/twister2016/twister)

## How To:
### Create AWS instances for Twister

To create required AWS instances for twister please follow [this guide](/documentation/Twister_AWS.md).

Also ensure to connect one additional NIC (from data Network) for use by Twister Application. The VMs will automatically reboot one time to apply updates required for Twister.

### Install Dependencies
Twister requires some basic build tools to be installed in Linux. 
Run following commands for debian based systems( Ubuntu, Debian etc.)
```bash
apt-get update -y
apt-get upgrade -y
apt-get install linux-generic linux-headers-generic make gcc gdb git unzip python-netaddr -y
bash -c "echo vm.nr_hugepages = 1024 >> /etc/sysctl.conf"
mkdir -p /mnt/huge
sed -i -e '$i \mount -t hugetlbfs nodev /mnt/huge &\n' /etc/rc.local
```

For yum based systems (CentOS, Fedora, Redhat etc.) run following commands
```bash
yum update -y
yum install make gcc gdb git unzip kernel-devel redhat-lsb -y
bash -c "echo vm.nr_hugepages = 1024 >> /etc/sysctl.conf"
mkdir -p /mnt/huge
chmod +x /etc/rc.local
sed -i -e '$i \mount -t hugetlbfs nodev /mnt/huge &\n' /etc/rc.local
bash -c "echo nodev /mnt/huge hugetlbfs defaults 0 0 >> /etc/fstab" 
```

Reboot the system for the changes to take effect
```
reboot
```
### Clone
The Twister source can be cloned using the following command:

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

Edit  twister configurations in `/etc/twister/twister.conf` file. `sudo` previleges will be required to edit this file.

Append comma separated names of interfaces to be used by Linux in `blacklist`. These interfaces will be kept binded to Linux drivers. Twister cannot use `blacklist` interfaces.

Similarly append comma separated names of interfaces to be used by Twister in `whitelist`. These interfaces will be binded with Twister and can be used by Twister applications. Linux applications cannot use `whitelist` interfaces.
Provide number of cores to be used by Twister in `cores` configuration. 

For example, final configuration file may look like below:
```
[DEFAULT]

blacklist=eth0
whitelist=eth1
cores=1
```
Edit port configurations in `/etc/twister/config.json`. `sudo` previleges will be required to edit this file.

Provide the network configurations for the NIC to be used as twister ports. The ports should have the configurations of network interfaces listed as `whitelist` interfaces in `/etc/twister/twister.conf` file. These NICs will be used in Twister applications as data network.

For example, run following command to configure tw0 port for twister.
```
sudo twifconfig tw0 10.0.2.129 255.255.255.0 10.0.2.1
```

Final configuration in `/etc/twister/config.json` file will look like below:
```
[{"ip_addrs": [{ "ip_address": "10.0.2.129", "gateway_ip": "10.0.2.1", "subnet_mask": "255.255.255.0"}], "port_name": "tw0"}]
```

To use multiple ports, edit the `/etc/twister/twister.conf` file with sudo privelages and append additional ports to the `whitelist`.
For example, if eth1 and eth2 are to be used by Twister, the `/etc/twister/twister.conf` file would look like:

```
[DEFAULT]

blacklist=eth0
whitelist=eth1,eth2
```
Network configuration for additional ports  would need to be added to `/etc/twister/config.json`.
For example, the `/etc/twister/config.json` file listing configurations for multiple two ports (added in the previous example) can be seen below.
It is important to ensure that the ports have different names:

```
[{"ip_addrs": [{ "ip_address": "10.0.2.129", "gateway_ip": "10.0.2.1", "subnet_mask": "255.255.255.0"}], "port_name": "tw0"},
{"ip_addrs": [{ "ip_address": "10.0.3.128", "gateway_ip": "10.0.3.1", "subnet_mask": "255.255.255.0"}], "port_name": "tw1"}]

``` 


Run `twifconfig -s`  to apply the changes.
```
sudo twifconfig -s
```
This script will apply the configurations defined in `/etc/twister/twister.conf` and `/etc/twister/config.json` files.

### Steps After Reboot
If the machine is rebooted after setting up Twister, do following steps to start Twister.
- Validate configurations in `/etc/twister/twister.conf` and `/etc/twister/config.json` files.
- Make sure that interfaces listed as `whitelist` in `/etc/twister/twister.conf` are do not have any IP address assigned by Linux.
- Run `twifconfif` to apply the changes
```
sudo twifconfig -s
```

## Applications:
- [Ping](documentation/example_applications.md#twping) This application sends the ICMP packets (ping), it takes IP from console, and sends the ICMP packets to this machine. When it gets the ICMP reply packet, it displays the ICMP parameters to user, like TTL, RTT, sequence number etc.
- [ICMP Echo](documentation/example_applications.md#twicmpecho) This application responds for the ICMP(Ping) packets.
- [ARP Application](/documentation/example_applications.md#twarp) This application implements the address resolution protocol. It sends the ARP request and displays the resolved MAC address.
- [iperf Application](documentation/example_applications.md#twiperf) This application provides functioanlity to measure maximum achievable bandwidth on IP conncection.




