TREE_ROOT?=$(DRIVER_TOPDIR)/
CONFIG_STGFB_PATH?=$(DRIVER_TOPDIR)/stgfb/stmfb

CCFLAGSY += -I$(TREE_ROOT)/player2/linux/include/linux/stm/
CCFLAGSY += -I$(TREE_ROOT)/player2/linux/drivers/media/video/stm/
CCFLAGSY += -I$(KERNEL_LOCATION)/drivers/media/dvb/dvb-core
CCFLAGSY += -I$(CONFIG_STGFB_PATH)/include
CCFLAGSY += -I$(TREE_ROOT)/include
CCFLAGSY += -DDEBUG_DVB

ifdef SPARK7162
CCFLAGSY += -I$(TREE_ROOT)/frontends/spark7162
CCFLAGSY += -I$(TREE_ROOT)/frontends/spark7162/include
CCFLAGSY += -I$(TREE_ROOT)/frontends/spark7162/demod/m3501
CCFLAGSY += -I$(TREE_ROOT)/frontends/spark7162/demod/stv0367
CCFLAGSY += -I$(TREE_ROOT)/frontends/spark7162/demod/stv0367/stv0367cab
CCFLAGSY += -I$(TREE_ROOT)/frontends/spark7162/demod/stv0367/stv0367ter
CCFLAGSY += -I$(TREE_ROOT)/frontends/spark7162/demod/mn88472
CCFLAGSY += -I$(TREE_ROOT)/frontends/spark7162/tuner
CCFLAGSY += -I$(TREE_ROOT)/frontends/spark7162/tuner/dummy
CCFLAGSY += -I$(TREE_ROOT)/frontends/spark7162/tuner/ix7306
CCFLAGSY += -I$(TREE_ROOT)/frontends/spark7162/tuner/vx7903
CCFLAGSY += -I$(TREE_ROOT)/frontends/spark7162/tuner/sn761640
CCFLAGSY += -I$(TREE_ROOT)/frontends/spark7162/tuner/MxL301_Ext
CCFLAGSY += -I$(TREE_ROOT)/frontends/spark7162/tuner/MxL603

CCFLAGSY += -Wall -DCONFIG_DVB_DUMMY_FE #-Werror

CCFLAGSY += -DSTB_BOARD_TYPE=1
CCFLAGSY += -DFULAN_3606_220_1CA_1CI=0
CCFLAGSY += -DFULAN_3606_220_SIMPLE=0
CCFLAGSY += -DSYS_TUN_MODULE=1
CCFLAGSY += -DMXL603=1
CCFLAGSY += -DANY_TUNER=0
endif

ccflags-y += $(CCFLAGSY)

# >= 2.6.30
ifdef HL101
fe-core-objs += hl101/stb0899_algo.o
fe-core-objs += hl101/stb0899_drv.o
fe-core-objs += hl101/stb6100.o
fe-core-objs += hl101/stv090x.o
fe-core-objs += hl101/stv6110x.o
fe-core-objs += hl101/ix7306.o
fe-core-objs += hl101/core.o
obj-m += fe-core.o
endif

ifdef VIP1_V1
fe-core-objs += hl101/stb0899_algo.o
fe-core-objs += hl101/stb0899_drv.o
fe-core-objs += hl101/stb6100.o
fe-core-objs += hl101/stv090x.o
fe-core-objs += hl101/stv6110x.o
fe-core-objs += hl101/ix7306.o
fe-core-objs += hl101/core.o
obj-m += fe-core.o
endif

ifdef VIP1_V2  # Same as VIP2
fe-core-objs += vip2/stv090x.o
fe-core-objs += vip2/stv6110x.o
fe-core-objs += vip2/ix7306.o
fe-core-objs += vip2/ce6353.o
fe-core-objs += vip2/tda10023.o
fe-core-objs += vip2/lg031.o
fe-core-objs += vip2/sharp6465.o
fe-core-objs += vip2/core.o
obj-m += fe-core.o
endif

ifdef VIP2
fe-core-objs += vip2/stv090x.o
fe-core-objs += vip2/stv6110x.o
fe-core-objs += vip2/ix7306.o
fe-core-objs += vip2/ce6353.o
fe-core-objs += vip2/tda10023.o
fe-core-objs += vip2/lg031.o
fe-core-objs += vip2/sharp6465.o
fe-core-objs += vip2/core.o
obj-m += fe-core.o
endif

ifdef SPARK
spark-fe-objs += spark/stv090x.o
spark-fe-objs += spark/stv6110x.o
spark-fe-objs += spark/ix7306.o
spark-fe-objs += spark/core.o
spark-fe-objs += spark/vx7903.o
obj-m += spark-fe.o
endif

ifdef SPARK7162
spark7162-fe-objs += spark7162/spark7162-fe.o
spark7162-fe-objs += spark7162/tuner/ioarch.o

# DVB-S(2) demodulator (all models)
spark7162-fe-objs += spark7162/demod/m3501/m3501.o

# DVB-T demodulator (early production)
spark7162-fe-objs += spark7162/demod/stv0367/stv0367.o
spark7162-fe-objs += spark7162/demod/stv0367/stv0367ter.o
spark7162-fe-objs += spark7162/demod/stv0367/stv0367cab.o

spark7162-fe-objs += spark7162/demod/stv0367/stv0367ter/chip_0367ter.o
spark7162-fe-objs += spark7162/demod/stv0367/stv0367ter/stv0367ter_drv.o
spark7162-fe-objs += spark7162/demod/stv0367/stv0367ter/stv0367_ter.o

spark7162-fe-objs += spark7162/demod/stv0367/stv0367cab/chip_0367cab.o
spark7162-fe-objs += spark7162/demod/stv0367/stv0367cab/stv0367cab_drv.o
spark7162-fe-objs += spark7162/demod/stv0367/stv0367cab/stv0367_cab.o

# DVB-T(2)/C demodulator (late production)
spark7162-fe-objs += spark7162/demod/mn88472/mn88472.o
spark7162-fe-objs += spark7162/demod/mn88472/mn88472_dvb.o
spark7162-fe-objs += spark7162/demod/mn88472/nim_mn88472.o
spark7162-fe-objs += spark7162/demod/mn88472/mn88472_autoctrl.o
spark7162-fe-objs += spark7162/demod/mn88472/mn88472_register.o
spark7162-fe-objs += spark7162/demod/mn88472/mn88472_variable.o

# DVB-S(2) tuner (early production)
spark7162-fe-objs += spark7162/tuner/ix7306/sharp_ix7306.o

# DVB-S(2) tuner (late production)
spark7162-fe-objs += spark7162/tuner/vx7903/sharp_vx7903.o

# DVB-T/C tuner (early production)
spark7162-fe-objs += spark7162/tuner/sn761640/sn761640t.o
spark7162-fe-objs += spark7162/tuner/sn761640/sn761640c.o

# DVB-T(2)/C tuner (late production)
spark7162-fe-objs += spark7162/tuner/MxL301_Ext/mxl301rf.o
spark7162-fe-objs += spark7162/tuner/MxL301_Ext/mxl_common.o
spark7162-fe-objs += spark7162/tuner/MxL301_Ext/mxl_spur_table.o
spark7162-fe-objs += spark7162/tuner/MxL301_Ext/mxL_user_define.o
spark7162-fe-objs += spark7162/tuner/MxL301_Ext/tun_mxl301.o

spark7162-fe-objs += spark7162/tuner/MxL603/MxL603_TunerApi.o
spark7162-fe-objs += spark7162/tuner/MxL603/MxL603_TunerCfg.o
spark7162-fe-objs += spark7162/tuner/MxL603/tun_mxl603.o

# Dummy front end to replace tuners not on main board
spark7162-fe-objs += spark7162/tuner/dummy/dvb_dummy_fe.o

obj-m += spark7162-fe.o
endif

ifdef UFS912
stv090x-objs += stv090x/stv090x.o
stv090x-objs += stv090x/stv6110x.o
stv090x-objs += stv090x/ix7306.o
stv090x-objs += stv090x/core.o
obj-m += stv090x.o
endif

ifdef UFS913
obj-y += multituner/
endif

ifdef ATEMIO520
stv090x-objs += stv090x/stv090x.o
stv090x-objs += stv090x/stv6110x.o
stv090x-objs += stv090x/ix7306.o
stv090x-objs += stv090x/core.o
obj-m += stv090x.o
obj-y += lnb/
endif

ifdef HS7110
stv090x-objs += stv090x/stv090x.o
stv090x-objs += stv090x/stv6110x.o
stv090x-objs += stv090x/ix7306.o
stv090x-objs +=stv090x/core.o
obj-m += stv090x.o
obj-y += lnb/
endif 

ifdef HS7119
stv090x-objs += stv090x/stv090x.o
stv090x-objs += stv090x/stv6110x.o
stv090x-objs += stv090x/ix7306.o
stv090x-objs +=stv090x/core.o
obj-m += stv090x.o
obj-y += lnb/
endif

ifdef HS7420
stv090x-objs += stv090x/stv090x.o
stv090x-objs += stv090x/stv6110x.o
stv090x-objs += stv090x/ix7306.o
stv090x-objs += stv090x/core.o
obj-m += stv090x.o
obj-y += lnb/
endif

ifdef HS7429
stv090x-objs += stv090x/stv090x.o
stv090x-objs += stv090x/stv6110x.o
stv090x-objs += stv090x/ix7306.o
stv090x-objs +=stv090x/core.o
obj-m += stv090x.o
obj-y += lnb/
endif 

ifdef HS7810A
stv090x-objs += stv090x/stv090x.o
stv090x-objs += stv090x/stv6110x.o
stv090x-objs += stv090x/ix7306.o
stv090x-objs += stv090x/core.o
obj-m += stv090x.o
obj-y += lnb/
endif

ifdef HS7819
stv090x-objs += stv090x/stv090x.o
stv090x-objs += stv090x/stv6110x.o
stv090x-objs += stv090x/ix7306.o
stv090x-objs +=stv090x/core.o
obj-m += stv090x.o
obj-y += lnb/
endif 

ifdef FS9000
stv090x-objs += stv090x/stv090x.o
stv090x-objs += stv090x/stv6110x.o
stv090x-objs += stv090x/ix7306.o
stv090x-objs += stv090x/core.o
obj-m += stv090x.o
endif

ifdef UFS922
cx21143-objs += cx24116/cx24116.o
obj-m += cx21143.o
ifeq (,$(wildcard $(DRIVER_TOPDIR)/frontends/avl2108_ufs922))
avl2108-objs += avl2108/avl2108.o
avl2108-objs += avl2108/stv6110a.o
avl2108-objs += avl2108/stv6306.o
avl2108-objs += avl2108/lnb_pio.o
#avl2108-objs += avl2108/lnbh221.o
avl2108-objs += avl2108/lnbh23.o
obj-m += avl2108.o
avl2108_platform-objs += avl2108/avl2108_platform.o
obj-m += avl2108_platform.o
else
avl2108-objs += avl2108_ufs922/avl2108.o
obj-m += avl2108.o
endif
endif

ifdef UFC960
tda10023-objs += ufc960/tda10023.o
tda10023-objs += ufc960/core.o
obj-m += tda10023.o
endif

ifdef UFS910
cx24116-objs += cx24116_dvbapi5/cx24116.o
obj-m += cx24116.o
endif

ifdef TF7700
cx24116-objs += cx24116_dvbapi5/cx24116.o
obj-m += cx24116.o
endif

ifdef HS9510
avl2108-objs += avl2108/avl2108.o
avl2108-objs += avl2108/stv6110a.o
avl2108-objs += avl2108/stv6306.o
avl2108-objs += avl2108/lnb_pio.o
avl2108-objs += avl2108/lnbh221.o
avl2108-objs += avl2108/lnbh23.o
obj-m += avl2108.o

avl2108_platform-objs += avl2108/avl2108_platform.o
obj-m += avl2108_platform.o
endif

ifdef HS8200
obj-y += multituner/
endif

ifdef IPBOX9900
fe-core-objs += ipbox/stv090x.o
fe-core-objs += ipbox/stv6110x.o
fe-core-objs += ipbox/zl10353.o
fe-core-objs += ipbox/mxl5007t.o
fe-core-objs += ipbox/tda10023.o
fe-core-objs += ipbox/core.o
obj-m += fe-core.o
endif

ifdef IPBOX99
fe-core-objs += ipbox/stv090x.o
fe-core-objs += ipbox/stv6110x.o
fe-core-objs += ipbox/zl10353.o
fe-core-objs += ipbox/mxl5007t.o
fe-core-objs += ipbox/tda10023.o
fe-core-objs += ipbox/core.o
obj-m += fe-core.o
endif

ifdef IPBOX55
fe-core-objs += ipbox/stv090x.o
fe-core-objs += ipbox/stv6110x.o
fe-core-objs += ipbox/core.o
obj-m += fe-core.o
endif

ifdef CUBEREVO
obj-y += multituner/
endif

ifdef CUBEREVO_MINI
obj-y += multituner/
endif

ifdef CUBEREVO_MINI2
obj-y += multituner/
endif

ifdef CUBEREVO_250HD
obj-y += multituner/
endif

ifdef CUBEREVO_9500HD
obj-y += multituner/
endif

ifdef CUBEREVO_2000HD
obj-y += multituner/
endif

ifdef CUBEREVO_MINI_FTA
obj-y += multituner/
endif

ifdef CUBEREVO_3000HD
obj-y += multituner/
endif

ifdef ADB_BOX
# Single ----------------------------------
# stb0899
fe-core-objs += adb_single/stb0899_algo.o
fe-core-objs += adb_single/stb0899_drv.o
fe-core-objs += adb_single/stb6100.o
fe-core-objs += adb_single/core.o
obj-m += fe-core.o

# Dual -------------------------------------
# stb0899
fe-core-twin1-objs += adb_dual/stb0899_algo.o
fe-core-twin1-objs += adb_dual/stb0899_drv.o
fe-core-twin1-objs += adb_dual/stb6100.o
fe-core-twin1-objs +=  adb_dual/core.o
obj-m += fe-core-twin1.o

# stv0900
fe-core-twin2-objs += adb_stv900/stv090x.o
fe-core-twin2-objs += adb_stv900/stb6100.o
fe-core-twin2-objs += adb_stv900/core.o
obj-m += fe-core-twin2.o

# DVB-C ----------------------------------
# stv0297 + mt2060
fe-mmp-objs += adb_mmp/mt2060.o
fe-mmp-objs += adb_mmp/stv0297.o
fe-mmp-objs += adb_mmp/core.o
obj-m += fe-mmp.o

#------------------------------------------------- 
endif

ifdef ADB_2850
# DVB-T ----------------------------------
# dip7000 + mxl111f
fe_core-objs += adb_2850/dibx000_common.o
fe_core-objs += adb_2850/dib0070.o
fe_core-objs += adb_2850/dib7000p.o

#fe_core-objs += adb_2850/mxl111sf-gpio.o
fe_core-objs += adb_2850/mxl111sf-phy.o
fe_core-objs += adb_2850/mxl111sf-demod.o
fe_core-objs += adb_2850/mxl111sf-tuner.o
fe_core-objs += adb_2850/mxl111sf.o

# DVB-S ----------------------------------
# stv090x + stv6100
fe_core-objs +=	adb_2850/stv090x.o
fe_core-objs +=	adb_2850/stv6110x.o
fe_core-objs +=	adb_2850/core.o
obj-m += fe_core.o
endif

ifdef VITAMIN_HD5000
stv090x-objs += vitamin_hd5000/stv090x.o
stv090x-objs += vitamin_hd5000/stv6110x.o
stv090x-objs += vitamin_hd5000/core.o
obj-m += stv090x.o
endif

ifdef ARIVALINK200
cx24116-objs += cx24116_dvbapi5/cx24116.o
obj-m += cx24116.o
endif

ifdef SAGEMCOM88
ifneq (,$(wildcard $(DRIVER_TOPDIR)/frontends/sagemcom88 ))
fe_core-objs += sagemcom88/tda18218.o
fe_core-objs += sagemcom88/tda10048.o
fe_core-objs += sagemcom88/stv090x.o
fe_core-objs += sagemcom88/stv6110x.o
fe_core-objs += sagemcom88/core.o
obj-m += fe_core.o
endif
endif

ifdef PACE7241
fe_core-objs += pace7241/tda18218.o
fe_core-objs += pace7241/tda10048.o
fe_core-objs += pace7241/stv090x.o
fe_core-objs += pace7241/stv6110x.o
fe_core-objs += pace7241/core.o
obj-m += fe_core.o
endif

ifdef OPT9600
opt9600_frontend-objs += opt9600/avl2108.o
opt9600_frontend-objs += opt9600/ix2470.o
opt9600_frontend-objs += opt9600/lnbp12.o
obj-m += opt9600_frontend.o

opt9600_platform-objs += opt9600/avl2108_platform.o
obj-m += opt9600_platform.o
endif

ifdef OPT9600MINI
stv090x-objs += stv090x/stv090x.o
stv090x-objs += stv090x/stv6110x.o
stv090x-objs += stv090x/ix7306.o
stv090x-objs += stv090x/core.o
obj-m += stv090x.o
obj-y += lnb/
endif

ifdef OPT9600PRIMA
opt9600_frontend-objs += opt9600/avl2108.o
opt9600_frontend-objs += opt9600/ix2470.o
opt9600_frontend-objs += opt9600/lnbp12.o
obj-m += opt9600_frontend.o

opt9600_platform-objs += opt9600/avl2108_platform.o
obj-m += opt9600_platform.o
endif

