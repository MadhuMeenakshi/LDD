#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/smp.h>

#define PROC_NAME "process_info"

static struct proc_dir_entry *proc_entry;

/* Read Function */
static ssize_t proc_read(struct file *file,
                         char __user *buffer,
                         size_t count,
                         loff_t *ppos)
{
    char message[256];
    int len;

    len = snprintf(message,
                   sizeof(message),
                   "PID            : %d\n"
                   "Parent PID     : %d\n"
                   "Process Name   : %s\n"
                   "Process State  : %u\n"
                   "Current CPU    : %u\n",
                   current->pid,
                   current->real_parent->pid,
                   current->comm,
                   current->__state,
                   raw_smp_processor_id());

    return simple_read_from_buffer(buffer,
                                   count,
                                   ppos,
                                   message,
                                   len);
}

/* proc Operations */
static const struct proc_ops proc_fops =
{
    .proc_read = proc_read,
};

/* Module Init */
static int __init process_info_init(void)
{
    proc_entry = proc_create(PROC_NAME,
                             0444,
                             NULL,
                             &proc_fops);

    if (!proc_entry)
    {
        printk(KERN_ERR "Failed to create /proc/%s\n", PROC_NAME);
        return -ENOMEM;
    }

    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);

    return 0;
}

/* Module Exit */
static void __exit process_info_exit(void)
{
    proc_remove(proc_entry);

    printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

module_init(process_info_init);
module_exit(process_info_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Madhu");
MODULE_DESCRIPTION("Process Information using procfs");
