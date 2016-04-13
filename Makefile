SUBDIR1 = dpdk
SUBDIR2	= twister
RTE_TARGET = x86_64-native-linuxapp-gcc

all: clean subdirs

dpdkconfig:
	cd dpdk \;
	make config T=x86_64-native-linuxapp-gcc \;
	cd .. \;

subdirs:
	$(MAKE) -C $(SUBDIR1);
	cp -R $(SUBDIR1)/build $(SUBDIR1)/$(RTE_TARGET);
	$(MAKE) -C $(SUBDIR2);

clean:
	rm -rf $(SUBDIR2)/build;
	rm -rf $(SUBDIR1)/build/build;
	rm -rf $(SUBDIR1)/$(RTE_TARGET);
