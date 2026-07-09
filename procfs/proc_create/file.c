#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

#define PROC_NAME "myproc"

static struct proc_dir_entry *proc_entry;

/* Read Function */
static ssize_t proc_read(struct file *file,
                         char __user *buffer,
                         size_t count,
                         loff_t *ppos)
{
    char message[] = "Hello from procfs!\n";

    return simple_read_from_buffer(buffer,
                                   count,
                                   ppos,
                                   message,
                                   sizeof(message));
}

/* File Operations */
static const struct proc_ops proc_fops =
{
    .proc_read = proc_read,
};

/* Module Init */
static int __init proc_init(void)
{
    proc_entry = proc_create(PROC_NAME,
                             0444,
                             NULL,
                             &proc_fops);

    if (!proc_entry)
    {
        printk(KERN_ERR "Failed to create proc entry\n");
        return -ENOMEM;
    }

    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);

    return 0;
}

/* Module Exit */
static void __exit proc_exit(void)
{
    proc_remove(proc_entry);

    printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Madhu");
MODULE_DESCRIPTION("Simple procfs Example");
