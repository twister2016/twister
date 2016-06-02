#Twister IPERF example application 

The example below will demonstrate to run Twister IPERF application developed in Twister.
The demo uses two VMs to run example application - one as Twister IPERF server and one as Twister IPERF client.
The demo can be run in simple three steps

1. Create two AWS instances for Twister
2. Configure Twister and ports configuration
3. Run twiperf (Twister Application)

##Create two AWS instances for Twister

Create two AWS instances "Twiperf Server" and "Twiperf Clinet". Please follow [this guide](/documentation/Twister_AWS.md) for creating AWS instances for Twister.

Also ensure to connect one extra NIC for use by Twister Application. The VMs will automatically reboot one time to apply updates required for Twister.

## Configure Twister and ports configurations

Clone Twister repository from github.
```
git clone https://www.github.com/twister2016/twister.git
```
compile and install Twister

```
cd twister
make
sudo make install
```
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
For example, in 'Twiiperf Server', final configuration in `/home/twister/config/config.json` file may look like below:
```
[{"ip_addrs": [{ "ip_address": "10.0.2.129", "gateway_ip": "10.10.2.1", "subnet_mask": "255.255.255.0"}], "port_name": "tw0"}]
```
Run `twister-config`  to apply the changes.
```
twister-config
```

This script will apply the configurations defined in '/home/twister/config/twister.conf' and generate a json file `/home/twister/config/twister_api.json` which will be used by Twister API.

## Run Twiperf (Twister Application)
go to twister repository folder and compile twister applications in `twister/applications` folders

```
cd twister/applications
sudo make install 
```
Run iperf server in 'Twiperf Server' instance with udp traffic.
```
sudo twiperf -u -s
```
Run iperf server in 'Twiperf client' instance. Connect with IP address of iperf server configured in 'Twipef server' VM.
```
sudo twiperf -u -c 10.0.2.129
```
