
KMOD := pcie_hotplug

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
CC := g++

all: $(KMOD).ko

$(KMOD).ko: driver/$(KMOD).c
	$(MAKE) -C $(KDIR) M=$(PWD)/driver modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD)/driver clean


install: $(KMOD).ko
	cp driver/$(KMOD).ko /lib/modules/$(shell uname -r)/kernel/drivers/misc/
	depmod
	modprobe $(KMOD)
	chmod 666 /dev/$(KMOD)_*

uninstall:
	modprobe -r $(KMOD)
	rm -f /lib/modules/$(shell uname -r)/kernel/drivers/misc/$(KMOD).ko
	depmod

.PHONY: all clean install uninstall