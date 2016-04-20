#!/bin/bash

KERNELSRC=""

function install_kernel()
{
    wget https://www.kernel.org/pub/linux/kernel/v3.x/linux-3.17.7.tar.gz
    tar xzvf linux-3.17.7.tar.gz > /dev/null
    cd linux-3.17.7
    make allmodconfig
    make oldconfig && make prepare
    make net/bridge/
    KERNELSRC=$(pwd)
    echo "Installed kernel source in $(pwd)"
    cd ..
}

function install_dpdk()
{
    cd dpdk
    find ./ -type f | xargs sed -i 's/max-inline-insns-single=100/max-inline-insns-single=400/'
    echo 'CONFIG_RTE_BUILD_FPIC=y' >>config/common_linuxapp
    sed -ri '/EXECENV_CFLAGS  = -pthread -fPIC/{s/$/\nelse ifeq ($(CONFIG_RTE_BUILD_FPIC),y)/;s/$/\nEXECENV_CFLAGS  = -pthread -fPIC/}' mk/exec-env/linuxapp/rte.vars.mk
    make config CC=gcc T=x86_64-native-linuxapp-gcc
    make CC=gcc RTE_KERNELDIR=$KERNELSRC
    sudo make install CC=gcc T=x86_64-native-linuxapp-gcc RTE_KERNELDIR=$KERNELSRC
    echo "Installed DPDK source in $(pwd)"
    cd ..
}

install_kernel
install_dpdk
