# Makefile

# Kernel module name
KMOD := pcie_hotplug
# User-space application name
APP := main

# Kernel source directory
KDIR := /lib/modules/$(shell uname -r)/build
# Current directory
PWD := $(shell pwd)

# Compiler
CC := g++

# Targets
all: $(KMOD).ko $(APP)

# Build the kernel module
$(KMOD).ko: driver/$(KMOD).c
	$(MAKE) -C $(KDIR) M=$(PWD)/driver modules

# Build the user-space application
$(APP): $(APP).cpp
	$(CC) -o $(APP) $(APP).cpp

# Clean up
clean:
	$(MAKE) -C $(KDIR) M=$(PWD)/driver clean
	rm -f $(APP)

# Install the kernel module
install: $(KMOD).ko
	cp driver/$(KMOD).ko /lib/modules/$(shell uname -r)/kernel/drivers/misc/
	depmod
	modprobe $(KMOD)
	chmod 666 /dev/$(KMOD)_*
	# echo "0000:21:00.0" | tee /sys/kernel/add_device
	# echo "0000:e2:00.0" | tee /sys/kernel/add_device
	# chmod 666 /dev/$(KMOD)_0000:21:00.0
	# chmod 666 /dev/$(KMOD)_0000:e2:00.0

# Uninstall the kernel module
uninstall:
	modprobe -r $(KMOD)
	rm -f /lib/modules/$(shell uname -r)/kernel/drivers/misc/$(KMOD).ko
	depmod

.PHONY: all clean install uninstall