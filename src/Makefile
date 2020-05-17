KEY_DIR ?= ./

obj-m += cpuautoscaling.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) -I/usr/include/ modules
	sudo /usr/src/linux-headers-$(shell uname -r)/scripts/sign-file sha256 $(KEY_DIR)/MOK.priv $(KEY_DIR)/MOK.der ./cpuautoscaling.ko

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

test: all
	# We put a — in front of the rmmod command to tell make to ignore
	# an error in case the module isn’t loaded.
	-sudo rmmod cpuautoscaling
	# Clear the kernel log without echo
	sudo dmesg -C
	# Insert the module
	sudo insmod cpuautoscaling.ko
	# Display the kernel log
	dmesg
