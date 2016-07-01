#/usr/bin/python
import json
import os
config_json = "/etc/twister/config.json"

class Interface():

    def __init__(self):
        self.name = ""
        self.ether = "00:00:00:00:00:00"
        self.inet = ""
        self.netmask = ""
        self.gateway = ""
        self.status = "UP"
        self.rx_pkts = 0
        self.tx_pkts = 0
        self.rx_bytes = 0
        self.tx_bytes = 0

    def print_interface(self):
        print self.name.ljust(10), "inet addr:", self.inet, "Mask:", self.netmask, "Gateway:", self.gateway
        print "".ljust(10), "Status:", self.status, "HWaddr", self.ether
        print "".ljust(10), "RX packets:", self.rx_pkts, "TX packets:".rjust(10), self.tx_pkts
        print "".ljust(10), "RX bytes:", self.rx_bytes,"TX bytes:", self.tx_bytes


def get_json():
    json_data = open(config_json, "r+")
    twister_json = json.load(json_data)
    json_data.close()
    return twister_json

def populate(print_flag=False):

    interfaces = []
    twister_json = get_json()
    for tw_interface in twister_json:
        interface  = Interface()
        interface.name = tw_interface["port_name"]
        interface.inet = tw_interface["ip_addrs"][0]["ip_address"]
        interface.netmask = tw_interface["ip_addrs"][0]["subnet_mask"]
        interface.gateway = tw_interface["ip_addrs"][0]["gateway_ip"]
        interface.ether = str(os.popen("sudo twgetstats " + tw_interface["port_name"]).read().rstrip())
        interfaces.append(interface)
    if print_flag:
        for twister_interface in interfaces:
            twister_interface.print_interface()
            print "\n"
    return interfaces

def print_interface (port_name):

    interfaces = populate ()
    port_found = False
    for tw_interface in interfaces:
         if tw_interface.name == port_name:
             tw_interface.print_interface()
             port_found = True
    if not port_found:
        message = str(port_name)+ " Device not found!"
        exit(message)

def configure_interface(port_name, ip_address, netmask, gateway):

    port_found = False
    interface = None
    twister_json = get_json()
    for i in range(len(twister_json)):
        if twister_json[i]["port_name"] == port_name:
            interface = Interface()
            interface.name = port_name
            interface.inet = ip_address
            twister_json[i]["ip_addrs"][0]["ip_address"] = ip_address
            interface.netmask = netmask
            twister_json[i]["ip_addrs"][0]["subnet_mask"] = netmask
            interface.gateway = gateway
            twister_json[i]["ip_addrs"][0]["gateway_ip"] = gateway
            port_found = True

    if not port_found:
        message = str(port_name)+ " Device not found!"
        exit(message)

    with open(config_json, 'w') as outfile:
        json.dump(twister_json, outfile)

    interface.print_interface()
