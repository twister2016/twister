SUBDIR1 = dpdk
SUBDIR2	= twister
RTE_TARGET = x86_64-native-linuxapp-gcc

all: clean dpdkconfig subdirs

dpdkconfig:
	./configure

subdirs:
	$(MAKE) -C $(SUBDIR1);
	cp -R $(SUBDIR1)/build $(SUBDIR1)/$(RTE_TARGET);
	$(MAKE) -C $(SUBDIR2);

clean:
	rm -rf $(SUBDIR2)/build;
	rm -rf $(SUBDIR1)/build/build;
	rm -rf $(SUBDIR1)/$(RTE_TARGET);

install: all
	cp $(SUBDIR2)/build/libtwister.a /home/twister/.
	cp $(SUBDIR1)/$(RTE_TARGET)/lib/* /home/twister/
	echo 'install all twister headers'
	cp $(SUBDIR2)/include/*.h /home/twister/include
	cp $(SUBDIR1)/$(RTE_TARGET)/include/*.h /home/twister/include
	cp $(SUBDIR1)/$(RTE_TARGET)/include/generic/*.h /home/twister/include/generic/
	cp $(SUBDIR1)/$(RTE_TARGET)/include/exec-env/*.h /home/twister/include/exec-env/
