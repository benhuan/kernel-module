print-%  : ; @echo $* = $($*)
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

#SRCs := $(wildcard *.c)
#OBJs := $(SRCs:.c=.o)

obj-m += $(MY_TARGET)

obj-m += hello_part.o
hello_part-objs := hello_part_1.o hello_part_2.o

all:
	make -C $(KDIR) M=$(PWD) modules
	rm *.o *.mod.c
clean:
	make -C $(KDIR) M=$(PWD) clean
