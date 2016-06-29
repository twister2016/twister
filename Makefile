SUBDIR1 = dpdk
SUBDIR2	= twister
SUBDIR3 = applications

RTE_TARGET = x86_64-native-linuxapp-gcc

DEB_DEPENDS  = make gcc

INSTALLED_DIR = /usr/lib/twister
INSTALLED_LIB = /usr/include/twister
CONFIGURATION_FILES = /etc/twister
KERNEL = $(shell uname -r)
MKDIR = mkdir -p

.PHONY: help bootstrap build clean all install install-lib uninstall rebuild-lib \
	applications

build: clean bootstrap build-local  build-application

mk_dir:
	$(MKDIR) $(INSTALLED_DIR)
	$(MKDIR) $(INSTALLED_DIR)/scripts
	$(MKDIR) $(INSTALLED_DIR)/modules
	$(MKDIR) $(INSTALLED_LIB)
	$(MKDIR) $(INSTALLED_LIB)/generic
	$(MKDIR) $(INSTALLED_LIB)/exec-env
	$(MKDIR) $(CONFIGURATION_FILES)


copy: mk_dir  
	echo 'Copying configuration files'
	cp $(SUBDIR2)/mk/tw_config.py $(INSTALLED_DIR)/scripts/tw_config.py
	cp $(SUBDIR2)/mk/insert_module.sh $(INSTALLED_DIR)/scripts/insert_module.sh
	cp $(SUBDIR1)/tools/dpdk_nic_bind.py $(INSTALLED_DIR)/scripts/dpdk_nic_bind.py
	cp -n $(SUBDIR2)/mk/twister.conf $(CONFIGURATION_FILES)/twister.conf
	cp -n $(SUBDIR2)/mk/config.json $(CONFIGURATION_FILES)/config.json
	cp -n $(SUBDIR2)/mk/twister_api.json $(CONFIGURATION_FILES)/twister_api.json
	echo 'Installing twister library files'
	cp $(SUBDIR2)/build/libtwister.a $(INSTALLED_DIR)/.
	cp $(SUBDIR1)/$(RTE_TARGET)/lib/* $(INSTALLED_DIR)
	echo 'install all twister headers'
	cp $(SUBDIR2)/include/*.h $(INSTALLED_LIB)
	cp $(SUBDIR1)/$(RTE_TARGET)/include/*.h $(INSTALLED_LIB)
	cp $(SUBDIR1)/$(RTE_TARGET)/include/generic/*.h $(INSTALLED_LIB)/generic/
	cp $(SUBDIR1)/$(RTE_TARGET)/include/exec-env/*.h $(INSTALLED_LIB)/exec-env/
	cp -n $(SUBDIR1)/$(RTE_TARGET)/kmod/igb_uio.ko $(INSTALLED_DIR)/modules/igb_uio.ko


build-local:  bootstrap
	$(MAKE) -C $(SUBDIR1);
	cp -R $(SUBDIR1)/build $(SUBDIR1)/$(RTE_TARGET);
	$(MAKE) -C $(SUBDIR2);

build-application:
	$(MAKE) -C $(SUBDIR3);

help:
	@echo "Make Targets:"
	@echo " bootstrap   - prepare system for twister installation"
	@echo " clean       - wipe all the package "
	@echo " all	     - wipe and build the whole package"
	@echo " install     - build and install the whole twister package"
	@echo " uninstall   - wipe and clean the system"
	@echo " build       - wipe and build the whole twister package"
	@echo " rebuild-lib - wipe and build the twister"
	@echo " install-lib - build and install the twister"
	@echo " applications - build and install twister example applications"

bootstrap:
ifeq ("$(shell lsb_release -si)", "Ubuntu")
	@MISSING=$$(apt-get install -y -qq -s $(DEB_DEPENDS) | grep "^Inst ") ; \
	if [ -n "$$MISSING" ] ; then \
	  echo "\nPlease install missing packages: \n$$MISSING\n" ; \
	  exit 1 ; \
	fi ; \
	exit 0
endif
	./configure

rebuild-lib:
	rm -rf $(SUBDIR2)/build;
	$(MAKE) -C $(SUBDIR2);

all: clean build-local

install-lib: rebuild-lib
	cp $(SUBDIR2)/build/libtwister.a $(INSTALLED_DIR).
	echo 'install all twister headers'
	cp $(SUBDIR2)/include/*.h $(INSTALLED_LIB)
	
clean:
	rm -rf $(SUBDIR2)/build;
	rm -rf $(SUBDIR1)/$(RTE_TARGET);

all: clean bootstrap build


install: copy
	cp $(SUBDIR2)/mk/twister.mk /usr/include0
	$(MAKE) install -C $(SUBDIR3);
	ln -s -f $(INSTALLED_DIR)/scripts/tw_config.py /usr/bin/twifconfig	

uninstall: clean 
	$(MAKE) uninstall -C $(SUBDIR3);
	rm -rf $(INSTALLED_LIB)
	rm -rf $(INSTALLED_DIR)
	rm -rf $(CONFIGURATION_FILES)
	rm -rf /lib/modules/$(KERNEL)/igb_uio.ko
       
