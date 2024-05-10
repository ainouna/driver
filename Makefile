# Tuxbox drivers Makefile
# there are only three targets
#
# make all     - builds all modules
# make install - installs the modules
# make clean   - deletes modules recursively
#
# note that "clean" only works in the current
# directory while "all" and "install" will
# execute from the top dir.

ifeq ($(KERNELRELEASE),)
DRIVER_TOPDIR:=$(shell pwd)
include $(DRIVER_TOPDIR)/kernel.make
else
CCFLAGSY += -D__TDT__ -D__LINUX__ -D__SH4__ -D__KERNEL__ -DMODULE -DEXPORT_SYMTAB -Wframe-larger-than=2048

ifdef HS9510
CCFLAGSY += -DHS9510
endif

ifdef UFS910
CCFLAGSY += -DUFS910
endif

ifdef CUBEREVO
CCFLAGSY += -DCUBEREVO
endif
ifdef CUBEREVO_MINI
CCFLAGSY += -DCUBEREVO_MINI
endif
ifdef CUBEREVO_MINI2
CCFLAGSY += -DCUBEREVO_MINI2
endif
ifdef CUBEREVO_250HD
CCFLAGSY += -DCUBEREVO_250HD
endif
ifdef CUBEREVO_2000HD
CCFLAGSY += -DCUBEREVO_2000HD
endif
ifdef CUBEREVO_9500HD
CCFLAGSY += -DCUBEREVO_9500HD
endif
ifdef CUBEREVO_MINI_FTA
CCFLAGSY += -DCUBEREVO_MINI_FTA
endif
ifdef CUBEREVO_3000HD
CCFLAGSY += -DCUBEREVO_3000HD
endif

ifdef TF7700
CCFLAGSY += -DTF7700
endif

ifdef HL101
CCFLAGSY += -DHL101
endif

ifdef VIP1_V1
CCFLAGSY += -DVIP1_V1
endif

ifdef VIP1_V2
CCFLAGSY += -DVIP1_V2
endif

ifdef VIP2
CCFLAGSY += -DVIP2
endif

ifdef UFS922
CCFLAGSY+=-DUFS922
endif

ifdef UFC960
CCFLAGSY+=-DUFC960
endif
ifdef UFS912
CCFLAGSY+=-DUFS912
endif
ifdef UFS913
CCFLAGSY+=-DUFS913
endif

ifdef SPARK
CCFLAGSY+=-DSPARK
endif
ifdef SPARK7162
CCFLAGSY+=-DSPARK7162
endif

ifdef FS9000
CCFLAGSY += -DFS9000
endif
ifdef HS8200
CCFLAGSY += -DHS8200
endif
ifdef HS7110
CCFLAGSY += -DHS7110
endif
ifdef HS7119
CCFLAGSY += -DHS7119
endif
ifdef HS7420
CCFLAGSY += -DHS7420
endif
ifdef HS7429
CCFLAGSY += -DHS7429
endif
ifdef HS7810A
CCFLAGSY += -DHS7810A
endif
ifdef HS7819
CCFLAGSY += -DHS7819
endif
ifdef ATEMIO520
CCFLAGSY += -DATEMIO520
endif
ifdef ADB_BOX
CCFLAGSY += -DADB_BOX
endif
ifdef ADB_2850
CCFLAGSY += -DADB_2850
endif

ifdef IPBOX9900
CCFLAGSY += -DIPBOX9900
endif
ifdef IPBOX99
CCFLAGSY += -DIPBOX99
endif
ifdef IPBOX55
CCFLAGSY += -DIPBOX55
endif

ifdef VITAMIN_HD5000
CCFLAGSY += -DVITAMIN_HD5000
endif
ifdef SAGEMCOM88
CCFLAGSY += -DSAGEMCOM88
endif
ifdef ARIVALINK200
CCFLAGSY += -DARIVALINK200
endif
ifdef PACE7241
CCFLAGSY += -DPACE7241
endif
ifdef OPT9600
CCFLAGSY += -DOPT9600
endif
ifdef OPT9600MINI
CCFLAGSY += -DOPT9600MINI
endif
ifdef OPT9600PRIMA
CCFLAGSY += -DOPT9600PRIMA
endif
ifdef HCHS8100
CCFLAGSY += -DHCHS8100
endif

ifneq (,$(findstring 2.6.3,$(KERNELVERSION)))
ccflags-y += $(CCFLAGSY)
else
CFLAGS += $(CCFLAGSY)
endif

export CCFLAGSY

obj-y := avs/
obj-y += multicom/
obj-y += stgfb/
obj-y += player2/

ifndef SAGEMCOM88 #Sagemcom88 has own boxtype
obj-y += boxtype/
endif
obj-y += cpu_frequ/
obj-y += simu_button/
obj-y += e2_proc/
obj-y += frontends/
obj-y += frontcontroller/
ifdef WLANDRIVER
obj-y += wireless/
endif

ifeq (,$(wildcard $(DRIVER_TOPDIR)/pti_np ))
obj-y += pti/
else
obj-y += pti_np/
endif

#obj-y += compcache/
obj-y += bpamem/

ifdef HL101
obj-y += smartcard/
endif

ifdef VIP1_V1
obj-y += smartcard/
endif

ifdef VIP1_V2
obj-y += smartcard/
endif

ifdef VIP2
obj-y += smartcard/
endif

ifdef ADB_BOX
#obj-y += boxtype/
obj-y += smartcard/
obj-y += fan_adb_box/
obj-y += cec_adb_box/
obj-y += frontends/dvbt/as102/
obj-y += frontends/dvbt/siano/
endif

ifdef ADB_2850
obj-y += smartcard/
obj-y += cec_adb_box/
#obj-y += dvbt/as102/
#obj-y += dvbt/siano/
endif

ifdef PACE7241
obj-y += smartcard/
endif

ifndef VIP2
ifndef SPARK
ifndef SPARK7162
ifndef CUBEREVO_MINI_FTA
ifndef CUBEREVO_250HD
obj-y += cic/
endif
endif
endif
endif
endif

# Button and Led Driver only needed for old 14W Kathrein Ufs 910 boxes
#ifdef UFS910
#obj-y += button/
#obj-y += led/
#endif

ifdef UFC960
obj-y += smartcard/
endif

ifdef UFS912
obj-y += cec/
endif

ifdef UFS913
obj-y += cec/
endif

ifdef HS8200
obj-y += cec/
obj-y += smartcard/
obj-y += sata_switch/
endif

ifdef HS7110
obj-y += cec/
obj-y += smartcard/
endif

ifdef HS7119
obj-y += cec/
obj-y += smartcard/
endif

ifdef HS7420
obj-y += cec/
obj-y += smartcard/
endif

ifdef HS7429
obj-y += cec/
obj-y += smartcard/
endif

ifdef HS7810A
obj-y += cec/
obj-y += smartcard/
endif

ifdef HS7819
obj-y += cec/
obj-y += smartcard/
endif

ifdef ATEMIO520
obj-y += cec/
obj-y += smartcard/
endif

ifdef SPARK
obj-y += cec/
obj-y += smartcard/
endif

ifdef SPARK7162
obj-y += smartcard/
#obj-y += i2c_spi/
obj-y += cec/
obj-y += rfmod/
obj-y += dvbt/as102/
obj-y += dvbt/siano/
endif

ifdef HS9510
obj-y += smartcard/
endif

ifdef FS9000
obj-y += smartcard/
endif

ifdef IPBOX9900
obj-y += siinfo/
obj-y += rmu/
obj-y += fan_ipbox99xx/
obj-y += smartcard/
obj-y += frontends/dvbt/as102/
endif

ifdef IPBOX99
obj-y += siinfo/
obj-y += fan_ipbox99xx/
obj-y += smartcard/
obj-y += frontends/dvbt/as102/
endif

ifdef IPBOX55
obj-y += siinfo/
obj-y += smartcard/
obj-y += frontends/dvbt/as102/
endif

ifdef CUBEREVO
obj-y += smartcard/
endif
ifdef CUBEREVO_MINI2
obj-y += smartcard/
endif
ifdef CUBEREVO_250HD
obj-y += smartcard/
endif
ifdef CUBEREVO_2000HD
obj-y += smartcard/
endif
ifdef CUBEREVO_9500HD
obj-y += smartcard/
endif
ifdef CUBEREVO_3000HD
obj-y += smartcard/
endif

ifdef TF7700
obj-y += tfswitch/
endif

ifdef VITAMIN_HD5000
obj-y += cec/
obj-y += smartcard/
endif

ifdef SAGEMCOM88
obj-y += cec/
obj-y += smartcard/
obj-y += sagemcomtype/
obj-y += fan_sagemcom88/
obj-y += dvbt/as102/
obj-y += dvbt/siano/
obj-y += frontends/dvbt/as102/
obj-y += frontends/dvbt/siano/
endif

ifdef ARIVALINK200
obj-y += smartcard/
obj-y += cec_adb_box/
obj-y += dvbt/as102/
obj-y += dvbt/siano/
obj-y += frontends/dvbt/as102/
obj-y += frontends/dvbt/siano/
endif

ifdef PACE7241
obj-y += cec/
obj-y += smartcard/
obj-y += fan_pace7241/
endif

ifdef OPT9600
#obj-y += cec/
obj-y += smartcard/
endif

ifdef OPT9600MINI
obj-y += cec/
obj-y += smartcard/
endif

ifdef OPT9600PRIMA
obj-y += cec/
obj-y += smartcard/
endif

ifdef HCHS8100
#obj-y += cec/
endif

endif
