#!/bin/sh
make clean
make
sudo mkdir -p /home/twister
sudo chmod 777 -R /home/twister
cp ./build/libtwister.a /home/twister/.
cp /usr/dpdk/x86_64-native-linuxapp-gcc/lib/* /home/twister/
echo 'install all twister headers'
mkdir -p /home/twister/include/.
cp  ./include/*.h /home/twister/include
cp /usr/dpdk/x86_64-native-linuxapp-gcc/include/*.h /home/twister/
mkdir -p /home/twister/generic/.
mkdir -p /home/twister/exec-env/.
cp /usr/dpdk/x86_64-native-linuxapp-gcc/include/generic/*.h /home/twister/generic/
cp /usr/dpdk/x86_64-native-linuxapp-gcc/include/exec-env/*.h /home/twister/exec-env/
echo "transfering twister.mk"
cp ./mk/twister.mk /home/twister/
echo "DONE........."
