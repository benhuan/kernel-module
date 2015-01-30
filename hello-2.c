/*  
 *  hello-2.c - Demonstrating the module_init() and module_exit() macros.
 *  This is preferred over using init_module() and cleanup_module().
 */
#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/init.h>		/* Needed for the macros */
#include <linux/time.h>

static int __init
hello_2_init (void)
{
  struct timespec ts_start, ts_end, res;
  long i = 10000000;

  printk (KERN_INFO "Hello, world 2\n");

  getnstimeofday (&ts_start);
  while (i--);
  getnstimeofday (&ts_end);

  printk (KERN_INFO "%lu\n", ts_end.tv_nsec - ts_start.tv_nsec);

  return 0;
}

static void __exit
hello_2_exit (void)
{
  printk (KERN_INFO "Goodbye, world 2\n");
}

module_init (hello_2_init);
module_exit (hello_2_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LongQi");
MODULE_DESCRIPTION("only for test");
