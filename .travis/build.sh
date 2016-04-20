#!/bin/bash

KERNELSRC=""

function install_kernel()
{
    wget https://www.kernel.org/pub/linux/kernel/v3.x/linux-3.14.7.tar.gz
    tar xzvf linux-3.14.7.tar.gz > /dev/null
    cd linux-3.14.7
    make allmodconfig
    KERNELSRC=$(pwd)
    echo "Installed kernel source in $(pwd)"
    cd ..
}


function install_dpdk()
{
    cd dpdk
    make config CC=gcc T=x86_64-native-linuxapp-gcc
    make CC=gcc RTE_KERNELDIR=$KERNELSRC
    sudo make install CC=gcc T=x86_64-native-linuxapp-gcc RTE_KERNELDIR=$KERNELSRC
    echo "Installed DPDK source in $(pwd)"
    cd ..
}

make help && make bootstrap && install_kernel && install_dpdk && make rebuild-lib
