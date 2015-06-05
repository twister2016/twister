rm -rf app
gcc test.c build/libtwister.a dpdk_libs/libdpdk.a -lpthread -lrt -ldl -I./include -I../dpdk/x86_64-native-linuxapp-gcc/include -o app
