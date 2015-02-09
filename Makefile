SRC = hello-5.c


obj-m += $(SRC:.c=.o)

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd) 

default:
	make -C $(KDIR) SUBDIRS=$(PWD) modules 
	rm *.o *.mod.c

clean:
	make -C $(KDIR) SUBDIRS=$(PWD) clean
