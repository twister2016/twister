#!/usr/bin/env python

import subprocess
import json
import ConfigParser
import dpdk_nic_bind as lib_dpdk

dpdk_drivers =["igb_uio", "vfio-pci", "uio_pci_generic"]

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




def get_kernel_drv(devices):

    global dpdk_drivers
    kernel_drv = []

    for d in devices.keys():
        if not lib_dpdk.has_driver(d):
            continue
        if devices[d]["Driver_str"] not in dpdk_drivers:
            kernel_drv.append(devices[d])
            return kernel_drv[0]['Driver_str']
    return None



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

def bind_device(cmd, device, driver):

    status = subprocess.Popen(['sudo', cmd, '-b', driver,  device], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    return status.communicate()

#def unbind_device(cmd, device, driver):
#
#    status = subprocess.Popen(['sudo', cmd, '-b', driver,  device], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
#    return status.communicate()

def get_total_cpus():

    cpu_status = subprocess.Popen(["/usr/bin/lscpu"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out, err = cpu_status.communicate()
    total_cpus = int(out.split()[11]) -1
    return total_cpus

def get_whitelist(devices, config):
    whitelist = config.get('DEFAULT', 'whitelist').split(',')
    whitelist_pci = []
    for device_name in whitelist:
        whitelist_pci.append(lib_dpdk.dev_id_from_dev_name(device_name))
    return whitelist_pci



def get_blacklist(devices, config):
    blacklist = config.get('DEFAULT', 'blacklist').split(',')
    blacklist_pci = []
    for device_name in blacklist:
        blacklist_pci.append(lib_dpdk.dev_id_from_dev_name(device_name))
    return blacklist_pci


def get_coremask(config):
    cores = int(config.get('DEFAULT', 'cores'))
    total_cpus = get_total_cpus()
    coremask = hex((2**total_cpus-1) << cores)
    return coremask

def parse_twister_params(twister_conf):
    pass

    config = ConfigParser.RawConfigParser()   
    config.readfp(open(twister_conf))

    blacklist = config.get('DEFAULT', 'blacklist').split(',')
    blacklist_pci = get_pci_address(sys_tree, blacklist)



    whitelist = config.get('DEFAULT', 'whitelist').split(',')
    whitelist_pci = get_pci_address(sys_tree, whitelist)



    cores = int(config.get('DEFAULT', 'cores'))
    coremask = hex((2**total_cpus-1) << cores)

    print "blacklist " , blacklist
    print "whitelist" , whitelist

    sys_tree = populate_lshw_paramas()

    if not set(whitelist).issubset([dev['logicalname'] for dev in sys_tree ]):
        raise Exception("whitelist interfaces in twister.conf do not exist")

    if not set(whitelist).isdisjoint(blacklist):
        raise Exception("whitelist conflicts with blacklist in twister.conf")
    total_cpus = get_total_cpus()
    portmask = hex(len(whitelist))


    return coremask, portmask, blacklist_pci, whitelist_pci

def bind_all_to_linux(cmd, devices):
    
    kernel_driver = get_kernel_drv(devices)
    for dev in devices.keys():
        bind_device(cmd, dev, kernel_driver)



def main():
    cmd = "/home/twister/config/dpdk_nic_bind.py"
    twister_conf = "/home/twister/config/twister.conf"
    twister_api = "/home/twister/config/twister_api.json"
    dpdk_drv = "igb_uio"


    lib_dpdk.check_modules()
    lib_dpdk.get_nic_details()
    devices = lib_dpdk.devices


    bind_all_to_linux(cmd, devices)

    config = ConfigParser.RawConfigParser()
    config.readfp(open(twister_conf))


    whitelist = get_whitelist(devices, config)
    blacklist = get_blacklist(devices, config)
    coremask = get_coremask(config)


    if not set(whitelist).isdisjoint(blacklist):
        raise Exception("whitelist conflicts with blacklist in twister.conf")


    if not set(whitelist).issubset(devices.keys()):
        raise Exception("whitelist interfaces in twister.conf do not exist")


    portmask = hex(len(whitelist))

    for device in whitelist:
        out, err = bind_device(cmd, device, dpdk_drv)

    twister_json = { "coremask": coremask, "portmask": portmask, "blacklist": blacklist}
    with open(twister_api, 'w') as outfile:
        json.dump(twister_json, outfile)

    print twister_json


if __name__ == '__main__':

    main()
