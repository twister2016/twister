#!/usr/bin/env python

import subprocess
import json
import ConfigParser
import dpdk_nic_bind as lib_dpdk
import shlex
import os
import sys
import argparse
import twister_interfaces as twconfig

kernel=str(os.popen("uname -r").read().rstrip())
dpdk_drivers = ["igb_uio", "vfio-pci", "uio_pci_generic"]
# from dpdk.org/doc/nics
supported_nic_drivers = ["ena", "cxgbe", "szedata2", "enic",
                         "oce", "e1000", "e1000e", "igb",
                         "ixgbe", "ixgbevf", "i40e", "fm10k", 
                         "mlx4", "mlx5", "nfp", "bnx2x", 
                         "virtio-net", "xenvirt", "vmxnet3", 
                         "vmxnet3-usermap", "memnic"
                         ]

def get_kernel_drv(devices):
    "Returns the name of Kernel NIC driver used by Linux"
    global dpdk_drivers
    kernel_drv = []

    for d in devices.keys():
        if not lib_dpdk.has_driver(d):
            continue
        if devices[d]["Driver_str"] not in dpdk_drivers:
            kernel_drv.append(devices[d])
            return kernel_drv[0]['Driver_str']
    return None


def bind_device(cmd, device, driver):
    "Binds device to given driver"
    status = subprocess.Popen(['sudo', cmd, '-b', driver,  device],
                              stdout=subprocess.PIPE,
                              stderr=subprocess.PIPE)
    return status.communicate()


def get_total_cpus():
    "Returns tolat number of CPUs available for the machine"
    lscpu_out = subprocess.Popen(shlex.split("/usr/bin/lscpu "),
                                 stdout=subprocess.PIPE,
                                 stderr=subprocess.PIPE)
    cpu_status = subprocess.Popen(shlex.split("grep CPU(s):"),
                                  stdin=lscpu_out.stdout,
                                  stdout=subprocess.PIPE,
                                  stderr=subprocess.PIPE)
    out, err = cpu_status.communicate()
    total_cpus = int(out.split()[1])
    return total_cpus


def get_whitelist(devices, config):
    "Return list of whitelist devices in bus:slot:func format"
    whitelist_pci = []
    try:
        whitelist = config.get('DEFAULT', 'whitelist').split(',')
        for device_name in whitelist:
            whitelist_pci.append(lib_dpdk.dev_id_from_dev_name(device_name))
    except:
        print "Preparing an empty whitelist"

    return whitelist_pci


def get_blacklist(devices, config):
    "Return list of blacklist devices in bus:slot:func format"
    blacklist_pci = []
    try:
        blacklist = config.get('DEFAULT', 'blacklist').split(',')
        for device_name in blacklist:
            blacklist_pci.append(lib_dpdk.dev_id_from_dev_name(device_name))
    except:
        print "Preparing an empty blacklist"

    return blacklist_pci


def get_coremask(config):
    "Returns CPU coremask usable by Twister"
    cores = int(config.get('DEFAULT', 'cores'))
    total_cpus = get_total_cpus()
    coremask = ''

    for i in range(total_cpus):
        if cores > i:
            coremask = coremask + "1"
        else:
            coremask = coremask + "0"

    return hex(int(coremask,2))

def get_portmask(whitelist):
    "Returns portmask usable by Twister"

    portmask = ""
    if len(whitelist) == 0:
        portmask = "0"

    for i in range(len(whitelist)):
        portmask = portmask + "1"
    return hex(int(portmask,2))


def bind_all_to_linux(cmd, devices):
    "Binds all available NICs to Linux NIC drivers"
    driver_dir = "/sys/bus/pci/drivers/"
    drivers = next(os.walk(driver_dir))[1]
    k_driver = get_kernel_drv(devices)
    driver_binded = False
    for dev in devices.keys():
        for kernel_driver in drivers:
            if kernel_driver == "igb_uio":
                continue
            if kernel_driver not in supported_nic_drivers:
                continue
            try:
                bind_device(cmd, dev, kernel_driver)
                break
            except:
                pass

def load_dpdk_module(dpdk_kernel_module):
    "Load DPDK kernel Modules to whom DPDK ports will be bind"

    subprocess.Popen(['sudo','/usr/lib/twister/scripts/insert_module.sh'],
                                 stdout=subprocess.PIPE,
                                 stderr=subprocess.PIPE)
def configure():

    cmd = "/usr/lib/twister/scripts/dpdk_nic_bind.py"
    twister_conf = "/etc/twister/twister.conf"
    twister_api = "/etc/twister/twister_api.json"
    dpdk_drv = "igb_uio"
    dpdk_kernel_module = "/usr/lib/twister/modules/igb_uio.ko"

    load_dpdk_module(dpdk_kernel_module)
    lib_dpdk.check_modules()
    lib_dpdk.get_nic_details()
    devices = lib_dpdk.devices

    bind_all_to_linux(cmd, devices)
    lib_dpdk.get_nic_details()
    config = ConfigParser.RawConfigParser()
    config.readfp(open(twister_conf))

    whitelist = get_whitelist(devices, config)
    blacklist = get_blacklist(devices, config)
    coremask = get_coremask(config)

    if not set(whitelist).isdisjoint(blacklist):
        raise Exception("whitelist conflicts with blacklist in twister.conf")

    if not set(whitelist).issubset(devices.keys()):
        raise Exception("whitelist interfaces in twister.conf do not exist")

    portmask = get_portmask(whitelist)
    for device in whitelist:
        out, err = bind_device(cmd, device, dpdk_drv)

    twister_json = {"coremask": coremask,
                    "portmask": portmask,
                    "blacklist": blacklist}
    with open(twister_api, 'w') as outfile:
        json.dump(twister_json, outfile)



def main():
    parser = argparse.ArgumentParser()   
    parser.add_argument("port_name", nargs='?')
    parser.add_argument("ip_addrs", nargs='?')
    parser.add_argument("subnet_mask", nargs='?')
    parser.add_argument("gateway_ip", nargs='?')
    parser.add_argument("-s", "--configure", action = "store_true")
    args = parser.parse_args()
    
    if args.configure:
        configure ()
        print ("\nTwister ports configured succesfully!\n")
        twconfig.populate(print_flag = True)
        sys.exit (0)
    elif len (sys.argv) == 1:
        twconfig.populate (print_flag = True)
    elif len(sys.argv) == 2:
        twconfig.print_interface (args.port_name)
    elif len (sys.argv) == 5:
        twconfig.configure_interface (args.port_name, args.ip_addrs, args.subnet_mask, args.gateway_ip)
    else:
        parser.print_help()
        sys.exit (0)
       

if __name__ == '__main__':

    if os.getuid() == 0:
        main()
    else:
        exit("\nYou need to have root privileges to run this script.\nPlease try again, this time using 'sudo'. Exiting.\n")
