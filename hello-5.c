/*
 *  hello-5.c -
 *  passing command line arguements to a module
 *
 */
#include <linux/init.h>   /* Needed for the macros */
#include <linux/kernel.h> /* Needed for KERN_INFO */
#include <linux/module.h> /* Needed by all modules */
#include <linux/moduleparam.h>
#include <linux/time.h>

#define MOD_VERSION 5

static short int myshort = 1;
static int myint = 326;
static long int mylong = 12398212;
static char *mystring = "mystring";
static int myintArray[] = {-1, 1, 2, 3, 5, 7};
static int arr_argc = 0;
/*
 * module_param(foo, int, 0000)
 * The first param is the parameters name
 * The second param is it's data type
 * The final argument is the permissions bits,
 * for exposing parameters in sysfs (if non-zero) at a later stage.
 */

module_param(myshort, short, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(myshort, "A short integer");
module_param(myint, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(myint, "An integer");
module_param(mylong, long, S_IRUSR);
MODULE_PARM_DESC(mylong, "A long integer");
module_param(mystring, charp, 0000);
MODULE_PARM_DESC(mystring, "A character string");

/*
 * module_param_array(name, type, num, perm);
 * The first param is the parameter's (in this case the array's) name
 * The second param is the data type of the elements of the array
 * The third argument is a pointer to the variable that will store the number
 * of elements of the array initialized by the user at module loading time
 * The fourth argument is the permission bits
 */
module_param_array(myintArray, int, &arr_argc, 0000);
MODULE_PARM_DESC(myintArray, "An array of integers");

static int __init hello_init(void) {
  struct timespec ts_start, ts_end;
  long i = 10000000;

  printk(KERN_INFO "Hello, world %d \n", MOD_VERSION);

  getnstimeofday(&ts_start);
  while (i--)
    ;
  getnstimeofday(&ts_end);

  printk(KERN_INFO "%lu\n", ts_end.tv_nsec - ts_start.tv_nsec);

  i = 0;
  printk(KERN_INFO "Hello, world 5\n=============\n");
  printk(KERN_INFO "myshort is a short integer: %hd\n", myshort);
  printk(KERN_INFO "myint is an integer: %d\n", myint);
  printk(KERN_INFO "mylong is a long integer: %ld\n", mylong);
  printk(KERN_INFO "mystring is a string: %s\n", mystring);
  for (i = 0; i < (sizeof myintArray / sizeof(int)); i++) {
    printk(KERN_INFO "myintArray[%d] = %d\n", i, myintArray[i]);
  }
  printk(KERN_INFO "got %d arguments for myintArray.\n", arr_argc);
  return 0;
}

static void __exit hello_exit(void) {
  printk(KERN_INFO "Goodbye,%d \n", MOD_VERSION);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LongQi");
MODULE_DESCRIPTION("only for test");
