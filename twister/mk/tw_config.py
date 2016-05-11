#!/usr/bin/env python

import subprocess
import json
import ConfigParser
import dpdk_nic_bind as lib_dpdk
import shlex

dpdk_drivers = ["igb_uio", "vfio-pci", "uio_pci_generic"]


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
    whitelist = config.get('DEFAULT', 'whitelist').split(',')
    whitelist_pci = []
    for device_name in whitelist:
        whitelist_pci.append(lib_dpdk.dev_id_from_dev_name(device_name))
    return whitelist_pci


def get_blacklist(devices, config):
    "Return list of blacklist devices in bus:slot:func format"
    blacklist = config.get('DEFAULT', 'blacklist').split(',')
    blacklist_pci = []
    for device_name in blacklist:
        blacklist_pci.append(lib_dpdk.dev_id_from_dev_name(device_name))
    return blacklist_pci


def get_coremask(config):
    "Returns CPU coremask usable by Twister"
    cores = int(config.get('DEFAULT', 'cores'))
    total_cpus = get_total_cpus()
    coremask = hex((2**total_cpus-1) << cores)
    return coremask


def bind_all_to_linux(cmd, devices):
    "Binds all available NICs to Linux NIC drivers"
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

    twister_json = {"coremask": coremask,
                    "portmask": portmask,
                    "blacklist": blacklist}
    with open(twister_api, 'w') as outfile:
        json.dump(twister_json, outfile)

    print twister_json


if __name__ == '__main__':

    main()
