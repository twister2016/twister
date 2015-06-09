cd ..
make clean
make
cd examples
rm -rf app
gcc test.c ../build/libtwister.a ../dpdk_libs/libdpdk.a -I../../dpdk/x86_64-native-linuxapp-gcc/include -lpthread -lrt -ldl -mssse3 -lm -o app
./app -c 1 -n 1 -- -p 1
