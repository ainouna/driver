TREE_ROOT?=$(DRIVER_TOPDIR)/
CCFLAGSY += -I$(TREE_ROOT)/frontends/multituner/frontends/avl2108/
CCFLAGSY += -I$(TREE_ROOT)/frontends/multituner/lnbs/
CCFLAGSY += -I$(TREE_ROOT)/frontends/multituner/platform/
CCFLAGSY += -I$(TREE_ROOT)/frontends/multituner/socket/
CCFLAGSY += -I$(TREE_ROOT)/frontends/multituner/include


ifneq (,$(findstring 2.6.3,$(KERNELVERSION)))
ccflags-y += $(CCFLAGSY)
else
CFLAGS += $(CCFLAGSY)
endif

lnb_pio-objs +=       lnbs/lnb_pio.o
obj-m += lnb_pio.o

lnbh221-objs +=       lnbs/lnbh221.o
obj-m += lnbh221.o

socket-objs +=       socket/socket.o
obj-m += socket.o

frontend_platform-objs +=       platform/frontend_platform.o
obj-m += frontend_platform.o

avl2108-objs +=        frontends/avl2108/avl2108.o
avl2108-objs +=        frontends/avl2108/stv6110a.o
avl2108-objs +=        frontends/avl2108/stv6306.o
obj-m += avl2108.o