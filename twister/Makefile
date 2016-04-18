	
SRC_ROOT:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
ifeq ($(RTE_SDK),)
RTE_SDK = $(SRC_ROOT)/../dpdk
endif
# Default target, can be overridden by command line or environment
RTE_TARGET = x86_64-native-linuxapp-gcc

include $(RTE_SDK)/mk/rte.vars.mk

# all source are stored in SRCS-y
SRCS-y := coremgmt/lcoreconf.c portmgmt/portconf.c memmgmt/mempoolconf.c event/event_loop.c timermodules/periodic_timers.c timermodules/timerdefs.c timermodules/timerfuncs.c \
 arp/arplogic.c rxtxmodule/queued_pkts.c stats/stats.c \
rxtxmodule/tx.c rxtxmodule/rx.c initmodule/initfuncs.c jsonmodule/jsonparser.c common/common.c jsonmodule/cJSON.c ethernet/eth.c coremgmt/processing_engines.c

CFLAGS += -DRTE_MACHINE_CPUFLAG_SSE -DRTE_MACHINE_CPUFLAG_SSE2 -DRTE_MACHINE_CPUFLAG_SSE3 -DRTE_MACHINE_CPUFLAG_SSSE3 -DRTE_MACHINE_CPUFLAG_SSE4_1 -DRTE_MACHINE_CPUFLAG_SSE4_2 -DRTE_MACHINE_CPUFLAG_AES -DRTE_MACHINE_CPUFLAG_PCLMULQDQ -DRTE_MACHINE_CPUFLAG_AVX -DRTE_MACHINE_CPUFLAG_RDRAND -DRTE_MACHINE_CPUFLAG_FSGSBASE -DRTE_MACHINE_CPUFLAG_F16C 
ALL_HEADERS = -I$(SRC_ROOT)/include #-I$(DPDK_HEADERS)
CFLAGS += $(WERROR_FLAGS)
CFLAGS += $(ALL_HEADERS)
CFLAGS += -O3
#CFLAGS += -g
LIB = libtwister.a
include $(RTE_SDK)/mk/rte.extlib.mk
