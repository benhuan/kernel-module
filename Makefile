obj-m += hello-1.o
obj-m += hello-2.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd) 

default:
	make -C $(KDIR) SUBDIRS=$(PWD) modules 
	rm *.o *.mod.c

clean:
	make -C $(KDIR) SUBDIRS=$(PWD) clean
