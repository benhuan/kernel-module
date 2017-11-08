#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/uaccess.h>

static char msg[1280];
static int last_len;

ssize_t read_proc(struct file *sp_file, char __user *buf, size_t size,
                  loff_t *offset) {
  int len = 0;
  struct task_struct *tasks;

	len += sprintf(msg + len, "The current process is:\n %s \t\t %d \n", current->comm, current->pid);

  for_each_process(tasks) {
    len += sprintf(msg + len, "%s \t\t %d \n", tasks->comm, tasks->pid);
  }

  if (last_len == len) {
    // finished reading
    last_len = 0;
    return 0;
  } else {
    copy_to_user(buf, msg, len);
		last_len = len;
    return len;
  }
}

struct file_operations fops = {.read = read_proc};

static int __init functn_init(void) {
  proc_create("ps_list", 0666, NULL, &fops);
  return 0;
}

static void __exit functn_cleanup(void) { remove_proc_entry("ps_list", NULL); }

MODULE_LICENSE("GPL");
module_init(functn_init);
module_exit(functn_cleanup);
