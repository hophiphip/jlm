module = jlm

device = /dev/$(module)
obj-m += $(module).o
 
.PHONY: device

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

install:
	sudo insmod $(module).ko 

device:
	sudo mknod $(device) c $(shell sudo dmesg | grep 'Registered device with major number' | awk 'END {print $$NF}') 0

remove:
	sudo rm $(device)
	sudo rmmod $(module)
