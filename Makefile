ifneq ($(KERNELRELEASE),)
    obj-m	:= generator_mod.o    
else
INCLUDE_MRAA := /root/mraa/api
LIB_MRAA := /root/mraa/build/src
SO_MRAA := mraa
KDIR	:= /lib/modules/$(shell uname -r)/build
PWD	:= $(shell pwd)
default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
test:
	gcc -o test -I$(INCLUDE_MRAA) -B$(LIB_MRAA) -l$(SO_MRAA) test.c
endif
