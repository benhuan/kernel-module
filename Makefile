print-%  : ; @echo $* = $($*)
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

#SRCs := $(wildcard *.c)
#OBJs := $(SRCs:.c=.o)

obj-m += $(MY_TARGET)


all:
	make -C $(KDIR) M=$(PWD) modules
	rm *.o *.mod.c
clean:
	make -C $(KDIR) M=$(PWD) clean
