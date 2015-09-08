#!/bin/bash
sudo modprobe uio
insmod $RTE_SDK/$RTE_TARGET/kmod/igb_uio.ko
sudo ifconfig eth1 0 down
sudo $RTE_SDK/tools/dpdk_nic_bind.py --bind=igb_uio eth1
sudo ./UDP_ECHO.exe
