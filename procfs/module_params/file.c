#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Madhu");
MODULE_DESCRIPTION("Module Parameter using procfs");

#define PROC_NAME "module_param"

/* Module Parameter */

static int value = 100;

module_param(value, int, 0644);
MODULE_PARM_DESC(value, "Integer Module Parameter");

static struct proc_dir_entry *proc_entry;

/* Read Function */

static ssize_t proc_read(struct file *file,
                         char __user *buffer,
                         size_t count,
                         loff_t *ppos)
{
    char message[100];
    int len;

    len = snprintf(message,
                   sizeof(message),
                   "Module Parameter = %d\n",
                   value);

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

static int __init module_param_init(void)
{
    proc_entry = proc_create(PROC_NAME,
                             0444,
                             NULL,
                             &proc_fops);

    if (!proc_entry)
        return -ENOMEM;

    printk(KERN_INFO "/proc/module_param created\n");

    return 0;
}

/* Module Exit */

static void __exit module_param_exit(void)
{
    proc_remove(proc_entry);

    printk(KERN_INFO "/proc/module_param removed\n");
}

module_init(module_param_init);
module_exit(module_param_exit);
