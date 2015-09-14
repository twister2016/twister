#   BSD LICENSE
#
#   Copyright(c) 2010-2014 Intel Corporation. All rights reserved.
#   All rights reserved.
#
#   Redistribution and use in source and binary forms, with or without
#   modification, are permitted provided that the following conditions
#   are met:
#
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in
#       the documentation and/or other materials provided with the
#       distribution.
#     * Neither the name of Intel Corporation nor the names of its
#       contributors may be used to endorse or promote products derived
#       from this software without specific prior written permission.
#
#   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
#   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
#   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
#   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
#   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
#   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
#   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
#   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	
SRC_ROOT:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
ifeq ($(RTE_SDK),)
$(error "Please define RTE_SDK environment variable")
endif
# Default target, can be overriden by command line or environment
RTE_TARGET = x86_64-native-linuxapp-gcc

include $(RTE_SDK)/mk/rte.vars.mk

# all source are stored in SRCS-y
SRCS-y := event/event_loop.c coremgmt/lcoreconf.c portmgmt/portconf.c memmgmt/mempoolconf.c ipv4/ip.c ipv4/udp.c timermodules/periodic_timers.c timermodules/timerdefs.c timermodules/timerfuncs.c \
ipv4/socket.c queue/simple_queue.c arp/arplogic.c rxtxmodule/queued_pkts.c pktctor/pktctor.c stats/stats.c \
rxtxmodule/tx.c rxtxmodule/rx.c initmodule/initfuncs.c jsonmodule/jsonparser.c common/common.c jsonmodule/cJSON.c ethernet/eth.c ethernet/vlan.c pktparser/pktparser.c coremgmt/processing_engines.c

CFLAGS += -DRTE_MACHINE_CPUFLAG_SSE -DRTE_MACHINE_CPUFLAG_SSE2 -DRTE_MACHINE_CPUFLAG_SSE3 -DRTE_MACHINE_CPUFLAG_SSSE3 -DRTE_MACHINE_CPUFLAG_SSE4_1 -DRTE_MACHINE_CPUFLAG_SSE4_2 -DRTE_MACHINE_CPUFLAG_AES -DRTE_MACHINE_CPUFLAG_PCLMULQDQ -DRTE_MACHINE_CPUFLAG_AVX -DRTE_MACHINE_CPUFLAG_RDRAND -DRTE_MACHINE_CPUFLAG_FSGSBASE -DRTE_MACHINE_CPUFLAG_F16C 
DPDK_HEADERS = $(SRC_ROOT)/../dpdk/x86_64-native-linuxapp-gcc/include
ALL_HEADERS = -I$(SRC_ROOT)/include -I$(DPDK_HEADERS)
CFLAGS += $(WERROR_FLAGS)
CFLAGS += $(ALL_HEADERS)
CFLAGS += -O3
CFLAGS += -g
LIB = libtwister.a
include $(RTE_SDK)/mk/rte.extlib.mk
