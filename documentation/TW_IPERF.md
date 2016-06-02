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

Please follow [this guide](/README.md) for configuring Twister.


## Run Twiperf (Twister Application)
Go to twister repository folder and compile twister applications in `twister/applications` folders

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
