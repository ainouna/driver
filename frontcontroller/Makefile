TREE_ROOT?=$(DRIVER_TOPDIR)/
CONFIG_STGFB_PATH?=$(DRIVER_TOPDIR)/stgfb/stmfb
CCFLAGSY += -I$(TREE_ROOT)/player2/linux/include/linux/stm
CCFLAGSY += -I$(TREE_ROOT)/player2/linux/drivers/media/video/stm
CCFLAGSY += -I$(KERNEL_LOCATION)/drivers/media/dvb/dvb-core
CCFLAGSY += -I$(CONFIG_STGFB_PATH)/include
CCFLAGSY += -DDEBUG_DVB
CCFLAGSY += -I$(TREE_ROOT)/include
CCFLAGSY += -I$(TREE_ROOT)/frontcontroller/include

ifneq (,$(findstring 2.6.3,$(KERNELVERSION)))
ccflags-y += $(CCFLAGSY)
else
CFLAGS += $(CCFLAGSY)
endif

ifdef UFS910
obj-y += ufs910_fp/
endif

ifdef UFS912
obj-y += kathrein_micom/
endif

ifdef UFS913
obj-y += kathrein_micom/
endif

ifdef UFS922
obj-y += kathrein_micom/
endif

ifdef UFC960
obj-y += kathrein_micom/
endif

ifdef TF7700
obj-y += tffp/
endif

ifdef HL101
obj-y += proton/
endif

ifdef VIP1_V1
obj-y += proton/
endif

ifdef VIP1_V2
obj-y += aotom_vip/
endif

ifdef VIP2
obj-y += aotom_vip/
endif

ifdef ADB_BOX
obj-y += adb_5800_fp/
#obj-y += adb_box_fp/front_bska/
#obj-y += adb_box_fp/front_bsla/
endif

ifdef ADB_2850
obj-y += adb_2850_fp/
endif

ifdef SPARK
obj-y += aotom_spark/
endif

ifdef SPARK7162
obj-y += aotom_spark/
endif

ifdef HS9510
obj-y += nuvoton/
endif

ifdef FS9000
obj-y += nuvoton/
endif

ifdef HS8200
obj-y += nuvoton/
endif

ifdef HS7110
obj-y += nuvoton/
endif

ifdef HS7119
obj-y += nuvoton/
endif

ifdef HS7420
obj-y += nuvoton/
endif

ifdef HS7429
obj-y += nuvoton/
endif

ifdef HS7810A
obj-y += nuvoton/
endif

ifdef HS7819
obj-y += nuvoton/
endif

ifdef ATEMIO520
obj-y += cn_micom/
endif

ifdef IPBOX9900
obj-y += ipbox99xx/
endif

ifdef IPBOX99
obj-y += ipbox99xx/
endif

ifdef IPBOX55
obj-y += ipbox55/
endif

ifdef CUBEREVO
obj-y += cuberevo_micom/
else ifdef CUBEREVO_MINI
obj-y += cuberevo_micom/
else ifdef CUBEREVO_MINI2
obj-y += cuberevo_micom/
else ifdef CUBEREVO_250HD
obj-y += cuberevo_micom/
else ifdef CUBEREVO_9500HD
obj-y += ipbox/
else ifdef CUBEREVO_2000HD
obj-y += cuberevo_micom/
else ifdef CUBEREVO_MINI_FTA
obj-y += cuberevo_micom/
else ifdef CUBEREVO_3000HD
obj-y += cuberevo_micom/
endif

ifdef VITAMIN_HD5000
obj-y += vitamin_hd5000/
endif

ifdef SAGEMCOM88
obj-y += front_led/
obj-y += front_vfd/
endif

ifdef ARIVALINK200
ifneq (,$(wildcard $(DRIVER_TOPDIR)/frontcontroller/front_ArivaLink200))
obj-y += front_ArivaLink200/
endif
endif

ifdef PACE7241
obj-y += pace_7241_fp/
endif

ifdef OPT9600
obj-y += opt9600_fp/
endif

ifdef OPT9600MINI
obj-y += cn_micom/
endif

ifdef OPT9600PRIMA
obj-y += cn_micom/
endif

