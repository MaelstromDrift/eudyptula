obj-m := misc_eud.o

all:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

test:
	sudo insmod misc_eud.ko
	sudo cat /dev/eud
	sudo rmmod misc_eud
	
clean:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
