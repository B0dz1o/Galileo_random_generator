ifneq ($(KERNELRELEASE),)
    obj-m	:= generator_mod.o
else
#KDIR	:= /lib/modules/$(shell uname -r)/build
ARCH := x86
CROSS_COMPILE := i586-poky-linux-
KDIR	:= /opt/clanton-full/1.4.2/sysroots/i586-poky-linux/usr/src/kernel
PWD	:= $(shell pwd)

MAKEARCH := $(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE)

default:
	$(MAKEARCH) -C $(KDIR) SUBDIRS=$(PWD) modules
clean:
	$(MAKEARCH) -C $(KDIR) SUBDIRS=$(PWD) clean
endif
