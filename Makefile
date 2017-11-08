print-%  : ; @echo $* = $($*)
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
ifndef MY_TARGET
    $(info Set your target by using export MY_TARGET=xxx.o)
    $(error MY_TARGET is undefined)
endif

#SRCs := $(wildcard *.c)
#OBJs := $(SRCs:.c=.o)

obj-m += $(MY_TARGET).o

# used for multiple files
# obj-m += hello_part.o
# hello_part-objs := hello_part_1.o hello_part_2.o

all:
	make -C $(KDIR) M=$(PWD) modules
	rm *.o *.mod.c
clean:
	make -C $(KDIR) M=$(PWD) clean
