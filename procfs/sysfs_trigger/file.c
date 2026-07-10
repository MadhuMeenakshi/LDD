#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Madhu");
MODULE_DESCRIPTION("procfs Statistics");

#define PROC_NAME "statistics"
#define BUFFER_SIZE 100

static struct proc_dir_entry *proc_entry;

static char message[BUFFER_SIZE];

static unsigned int read_count;
static unsigned int write_count;

/* Read */

static ssize_t proc_read(struct file *file,
                         char __user *buffer,
                         size_t count,
                         loff_t *ppos)
{
    char output[100];
    int len;

    read_count++;

    len = snprintf(output,
                   sizeof(output),
                   "Reads  : %u\n"
                   "Writes : %u\n",
                   read_count,
                   write_count);

    return simple_read_from_buffer(buffer,
                                   count,
                                   ppos,
                                   output,
                                   len);
}

/* Write */

static ssize_t proc_write(struct file *file,
                          const char __user *buffer,
                          size_t count,
                          loff_t *ppos)
{
    if (count >= BUFFER_SIZE)
        count = BUFFER_SIZE - 1;

    if (copy_from_user(message, buffer, count))
        return -EFAULT;

    message[count] = '\0';

    write_count++;

    printk(KERN_INFO "Message : %s\n", message);

    return count;
}

/* Operations */

static const struct proc_ops proc_fops =
{
    .proc_read  = proc_read,
    .proc_write = proc_write,
};

/* Module Init */

static int __init statistics_init(void)
{
    proc_entry = proc_create(PROC_NAME,
                             0666,
                             NULL,
                             &proc_fops);

    if (!proc_entry)
        return -ENOMEM;

    printk(KERN_INFO "/proc/statistics created\n");

    return 0;
}

/* Module Exit */

static void __exit statistics_exit(void)
{
    proc_remove(proc_entry);

    printk(KERN_INFO "/proc/statistics removed\n");
}

module_init(statistics_init);
module_exit(statistics_exit);
