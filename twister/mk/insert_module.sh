#!/bin/sh
sudo modprobe uio
a=`lsmod | grep igb_uio | wc -l`
if [ "$a" -eq "2" ]
then
    echo "Module already inserted"
else
    echo "Inserting Modules in kernel"
    sudo insmod /usr/lib/twister/modules/igb_uio.ko
fi
