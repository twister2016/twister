#!/usr/bin/env python

import subprocess
import json
import ConfigParser


def populate_lshw_paramas():
    ports = []
    proc = subprocess.Popen(['lshw','-json'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out, err = proc.communicate()
    sys_tree = json.loads(out)

    for boards in sys_tree["children"]:
        for devices in boards["children"]:
            if devices["id"]=="pci":
                for pcichild in devices["children"]:
                    if pcichild["class"]=="network":
                        tempdic={}
                        pciaddr = pcichild["handle"]
                        tempdic["pciaddress"]=pciaddr[4:]
                        tempdic["logicalname"]=pcichild["logicalname"]
                        ports.append(tempdic)

    return ports

def get_pci_address(sys_tree, interface_list):
    pci_list = []
    for device in sys_tree:
        if "logicalname" in device and device['logicalname'] in interface_list:
            pci_list.append(device['pciaddress'])
    return pci_list

def check_bind_devices(cmd):

    bind_dpdk_devices = []
    status = subprocess.Popen([cmd, '--status'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out, err = status.communicate()

    for line in out.split('\n')[3:]:
        if line == "<none>" or line == '':
            break
        else:
            bind_dpdk_devices.append(line.split()[0])
    return bind_dpdk_devices

def unbind_device(cmd, device):

    status = subprocess.Popen([cmd, '-b', 'ixgbevf',  device], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    return status.communicate()

def get_total_cpus():

    cpu_status = subprocess.Popen(["/usr/bin/lscpu"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out, err = cpu_status.communicate()

    total_cpus = int(out.split()[11]) -1
    return total_cpus

def parse_twister_params(twister_conf):

    config = ConfigParser.RawConfigParser()   
    config.readfp(open(twister_conf)

    blacklist = config.get('DEFAULT', 'blacklist').split(',')
    whitelist = config.get('DEFAULT', 'whitelist').split(',')
    cores = int(config.get('DEFAULT', 'cores'))

    print "blacklist " , blacklist
    print "whitelist" , whitelist

    sys_tree = populate_lshw_paramas()
    blacklist_pci = get_pci_address(sys_tree, blacklist)

    if not set(whitelist).issubset([dev['logicalname'] for dev in sys_tree ]):
        raise Exception("whitelist interfaces in twister.conf do not exist")

    if not set(whitelist).isdisjoint(blacklist):
        raise Exception("whitelist conflicts with blacklist in twister.conf")
    total_cpus = get_total_cpus()
    coremask = hex((2**total_cpus-1) << cores)
    portmask = hex(len(whitelist))


    return coremask, portmask, blacklist_pci



if __name__ == '__main__':

    cmd = "/home/twister/config/dpdk_nic_bind.py"
    twister_conf = "/home/twister/config/twister.conf"
    twister_api = "/home/twister/config/twister_api.json"

    bind_dpdk_dev = check_bind_devices(cmd)
    print bind_dpdk_dev

    for device in bind_dpdk_dev:
        out, err = unbind_device(cmd, device)

    coremask, portmask, blacklist = parse_twister_params(twister_conf)

    twister_json = { "coremask": coremask, "portmask": portmask, "blacklist": blacklist}
    with open(twister_api, 'w') as outfile:
        json.dump(twister_json, outfile)

    print twister_json
