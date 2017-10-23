# kernel-module
The Linux kernel module progamming practice.

### Compile the module
```
export MY_TARGET=proc-2.o
make
```
We can get the compiled module `proc-2.ko`.

### Load the module: 
```
sudo insmod ./proc-2.ko
```

### Remove the module 
```
sudo rmmod proc-2
```
