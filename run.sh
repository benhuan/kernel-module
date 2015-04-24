sudo rmmod $1
sudo insmod $1.ko mylong=123987 mystring="zhanglongqi--++\\\\" myshort=251
tail -n 20 /var/log/kern.log
sudo rmmod $1
