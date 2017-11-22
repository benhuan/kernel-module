/*
 *  sched.c - scheduale a function to be called on every timer interrupt.
 *
 *  Copyright (C) 2001 by Peter Jay Salzman
 */

/*
 * The necessary header files
 */

/*
 * Standard in kernel modules
 */
#include <linux/kernel.h>    /* We're doing kernel work */
#include <linux/module.h>    /* Specifically, a module */
#include <linux/proc_fs.h>   /* Necessary because we use the proc fs */
#include <linux/sched.h>     /* We need to put ourselves to sleep*/
#include <linux/workqueue.h> /* We scheduale tasks here */
                             /* and wake up later */
#include <linux/init.h>      /* For __init and __exit */
#include <linux/interrupt.h> /* For irqreturn_t */
#include <linux/uaccess.h>

#include <linux/version.h>

#define PROC_ENTRY_FILENAME "sched"
#define MY_WORK_QUEUE_NAME "WQsched.c"

/*
 * The number of times the timer interrupt has been called so far
 */
static int counter_0 = 0;

static void work_routine(struct work_struct *);

static int die = 0; /* set this to 1 for shutdown */
static unsigned long onesec;
static int len_check = 1;

/*
 * The work queue structure for this task, from workqueue.h
 */
static struct workqueue_struct *wq;

static struct delayed_work my_work;

/*
 * This function will be called on every timer interrupt. Notice the void*
 * pointer - task functions can be used for more than one purpose, each time
 * getting a different parameter.
 */
static void work_routine(struct work_struct *work) {
  counter_0++;
  printk(KERN_INFO "my task from sched:%d", counter_0);
  /*
   * If cleanup wants us to die
   */
  if (die == 0)
    queue_delayed_work(wq, &my_work, onesec);
}

/*
 * Put data into the proc fs file.
 */
ssize_t procfile_read(struct file *sp_file, char __user *buf, size_t size,
                      loff_t *offset) {
  int len; /* The number of bytes actually used */
           /*
            * It's static so it will still be in memory
            * when we leave this function
            */
  static char my_buffer[80];

  if (len_check) {
    printk(KERN_INFO "Proc start reading: %lld", *offset);
    len_check = 0;
  } else {
    printk(KERN_ALERT "Proc stop reading: %lld", *offset);
    len_check = 1;
    return 0;
  }
  /*
   * Fill the buffer and get its length
   */
  len = sprintf(my_buffer, "Timer called %d times so far\n", counter_0);
  printk(KERN_INFO "Proc reading: %s", my_buffer);

  copy_to_user(buf, my_buffer, 80);
  return len;
}

struct file_operations fops = {.read = procfile_read};

/*
 * Initialize the module - register the proc file
 */
int __init init_module() {

  onesec = msecs_to_jiffies(1000);
  /*
* Create our /proc file
*/

  if (!proc_create(PROC_ENTRY_FILENAME, 0644, NULL, &fops)) {
    remove_proc_entry(PROC_ENTRY_FILENAME, NULL);
    printk(KERN_ALERT "Error: Could not initialize /proc/%s\n",
           PROC_ENTRY_FILENAME);
    return -ENOMEM;
  }

/*
 * Put the task in the work_timer task queue, so it will be executed at
 * next timer interrupt
 */

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 37))
  wq = alloc_workqueue(MY_WORK_QUEUE_NAME, 0, 0);
#else
  wq = create_workqueue(MY_WORK_QUEUE_NAME);
#endif

  if (!wq) {
    printk(KERN_ALERT "Alloc/Create workqueue failed on kernel version %d\n",
           LINUX_VERSION_CODE);
    goto error_out;
  }

	INIT_DELAYED_WORK(&my_work, work_routine);

  queue_delayed_work(wq, &my_work, onesec);

  printk(KERN_INFO "/proc/%s created\n", PROC_ENTRY_FILENAME);

  return 0;
error_out:
  return -1;
}

/*
 * Cleanup
 */
void __exit cleanup_module() {
  /*
   * Unregister our /proc file
   */
  remove_proc_entry(PROC_ENTRY_FILENAME, NULL);
  printk(KERN_INFO "/proc/%s removed\n", PROC_ENTRY_FILENAME);

  die = 1;                       /* keep intrp_routine from queueing itself */
  cancel_delayed_work(&my_work); /* no "new ones" */
  flush_workqueue(wq);           /* wait till all "old ones" finished */
  destroy_workqueue(wq);

  /*
   * Sleep until work_routine is called one last time. This is
   * necessary, because otherwise we'll deallocate the memory holding
   * work_routine and my_work while work_timer still references them.
   * Notice that here we don't allow signals to interrupt us.
   *
   * Since WaitQ is now not NULL, this automatically tells the interrupt
   * routine it's time to die.
   */
}

/*
 * some work_queue related functions
 * are just available to GPL licensed Modules
 */
MODULE_LICENSE("GPL");
