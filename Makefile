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
<<<<<<< HEAD
	rm -rf $(SUBDIR2)/build;
	rm -rf $(SUBDIR1)/build/build;
=======
	rm -rf $(SUBDIR1)/build;
	rm -rf $(SUBDIR2)/build;
>>>>>>> b756bccf00526d6d21c9853ec9cf195d4b77986e
	rm -rf $(SUBDIR1)/$(RTE_TARGET);
