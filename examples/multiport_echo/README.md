= Multiport Ethernet Echo
:docinfo:
[preface]

The multiport example application is a example of packet processing built on Twister API which also takes advantage of more than one ethernet port.
== OVerview
The multiport example application, performs L2 forwarding for each packet that is received on the ports and  Also, the MAC addresses are affected as follows:

The source MAC address is replaced by the TX_PORT MAC address
The destination MAC address is replaced by the Source MAC address

== Compiling and Running the Application
Build the application using the following command:
make
To run the application in linuxapp environment
./MUL_Echo
