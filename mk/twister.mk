TWISTER_SDK:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
IDIR =$(TWISTER_SDK)/../twister/include
IDIR2 = $(RTE_SDK)/x86_64-native-linuxapp-gcc/include

LDIR =$(RTE_SDK)/x86_64-native-linuxapp-gcc/lib
TWISTERLIB =$(TWISTER_SDK)/../twister/build/libtwister.a



TWFLAGS=-g -m64 -pthread $(TWISTERLIB) -L$(LDIR) -I$(IDIR) -I$(IDIR2) -lpthread -lrt -ldl -mssse3 -lm -Wl,--whole-archive -Wl,-lrte_distributor -Wl,-lrte_reorder -Wl,-lrte_kni -Wl,-lrte_pipeline -Wl,-lrte_table -Wl,-lrte_port -Wl,-lrte_timer -Wl,-lrte_hash -Wl,-lrte_jobstats -Wl,-lrte_lpm -Wl,-lrte_power -Wl,-lrte_acl -Wl,-lrte_meter -Wl,-lrte_sched -Wl,-lm -Wl,-lrt -Wl,--start-group -Wl,-lrte_kvargs -Wl,-lrte_mbuf -Wl,-lrte_ip_frag -Wl,-lethdev -Wl,-lrte_malloc -Wl,-lrte_mempool -Wl,-lrte_ring -Wl,-lrte_eal -Wl,-lrte_cmdline -Wl,-lrte_cfgfile -Wl,-lrte_pmd_bond -Wl,-lrte_pmd_vmxnet3_uio -Wl,-lrte_pmd_virtio_uio -Wl,-lrte_pmd_enic -Wl,-lrte_pmd_i40e -Wl,-lrte_pmd_fm10k -Wl,-lrte_pmd_ixgbe -Wl,-lrte_pmd_e1000 -Wl,-lrte_pmd_ring -Wl,-lrte_pmd_af_packet -Wl,-lrt -Wl,-lm -Wl,-ldl -Wl,--end-group -Wl,--no-whole-archive
