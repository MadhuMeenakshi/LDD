#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

#define PROC_NAME "counter"

static struct proc_dir_entry *proc_entry;
static int counter = 0;

/* Read Function */
static ssize_t proc_read(struct file *file,
                         char __user *buffer,
                         size_t count,
                         loff_t *ppos)
{
    char message[20];
    int len;

    counter++;

    len = snprintf(message, sizeof(message), "%d\n", counter);

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

/* Init */
static int __init counter_init(void)
{
    proc_entry = proc_create(PROC_NAME,
                             0444,
                             NULL,
                             &proc_fops);

    if (!proc_entry)
    {
        printk(KERN_ERR "Cannot create proc entry\n");
        return -ENOMEM;
    }

    printk(KERN_INFO "/proc/counter created\n");

    return 0;
}

/* Exit */
static void __exit counter_exit(void)
{
    proc_remove(proc_entry);

    printk(KERN_INFO "/proc/counter removed\n");
}

module_init(counter_init);
module_exit(counter_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Madhu");
MODULE_DESCRIPTION("Counter using procfs");
