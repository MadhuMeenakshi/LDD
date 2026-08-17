#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

#define PROC_NAME "message"
#define BUFFER_SIZE 100

static struct proc_dir_entry *proc_entry;
static char message[BUFFER_SIZE] = "Hello";
static size_t message_len;

/* Read Function */
static ssize_t proc_read(struct file *file,
                         char __user *buffer,
                         size_t count,
                         loff_t *ppos)
{
    return simple_read_from_buffer(buffer,
                                   count,
                                   ppos,
                                   message,
                                   message_len);
}

/* Write Function */
static ssize_t proc_write(struct file *file,
                          const char __user *buffer,
                          size_t count,
                          loff_t *ppos)
{
    if (count > BUFFER_SIZE - 1)
        count = BUFFER_SIZE - 1;

    if (copy_from_user(message, buffer, count))
        return -EFAULT;

    message[count] = '\0';
    message_len = count;

    printk(KERN_INFO "Message Received : %s\n", message);

    return count;
}

/* Operations */
static const struct proc_ops proc_fops =
{
    .proc_read = proc_read,
    .proc_write = proc_write,
};

/* Init */
static int __init proc_init(void)
{
    message_len = strlen(message);

    proc_entry = proc_create(PROC_NAME,
                             0666,
                             NULL,
                             &proc_fops);

    if (!proc_entry)
    {
        printk(KERN_ERR "Cannot create proc entry\n");
        return -ENOMEM;
    }

    printk(KERN_INFO "/proc/message created\n");

    return 0;
}

/* Exit */
static void __exit proc_exit(void)
{
    proc_remove(proc_entry);

    printk(KERN_INFO "/proc/message removed\n");
}

module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Madhu");
MODULE_DESCRIPTION("procfs Read Write Example");
