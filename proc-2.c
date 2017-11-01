#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/types.h>
#include <linux/uaccess.h>

static char msg[128];
static int len = 0;
static int len_check = 1;

static int my_proc_show(struct seq_file *seq, void *v) {
  seq_printf(seq, "Print something from kernel module proc-2\n");
  seq_puts(seq, msg);
  return 0;
}
static int my_proc_open(struct inode *inode, struct file *file) {
  return single_open(file, my_proc_show, NULL);
}
int simple_proc_release(struct inode *sp_indoe, struct file *sp_file) {
  printk(KERN_INFO "proc called release\n");
  return 0;
}

static ssize_t simple_proc_write(struct file *sp_file, const char __user *buf,
                                 size_t size, loff_t *offset) {
  printk(KERN_INFO "proc called write %d\n", size);
  len = size;
  copy_from_user(msg, buf, len);
  return len;
}

static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = my_proc_open,
    .read = seq_read,
    .write = simple_proc_write,
    .llseek = seq_lseek,
    .release = single_release,
};
static int __init init_simpleproc(void) {
  printk(KERN_INFO "init simple proc\n");
  if (!proc_create("simpleproc", 0666, NULL, &fops)) {
    printk(KERN_INFO "ERROR! proc_create\n");
    remove_proc_entry("simpleproc", NULL);
    return -1;
  }
  return 0;
}
static void __exit exit_simpleproc(void) {
  remove_proc_entry("simpleproc", NULL);
  printk(KERN_INFO "exit simple proc\n");
}

module_init(init_simpleproc);
module_exit(exit_simpleproc);
MODULE_AUTHOR("Soorej P");
MODULE_LICENSE("GPL v3");
MODULE_DESCRIPTION("A simple module to input/output using proc filesystem");
