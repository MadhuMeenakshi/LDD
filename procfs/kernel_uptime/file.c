#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/ktime.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Madhu");
MODULE_DESCRIPTION("Kernel Uptime using procfs");

#define PROC_NAME "uptime_test"

static struct proc_dir_entry *proc_entry;

/* Read Function */
static ssize_t proc_read(struct file *file,
                         char __user *buffer,
                         size_t count,
                         loff_t *ppos)
{
    char message[100];
    int len;
    u64 uptime;

    /* Get uptime in seconds */
    uptime = ktime_get_boottime_seconds();

    len = snprintf(message,
                   sizeof(message),
                   "Kernel Uptime : %llu seconds\n",
                   uptime);

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

static int __init uptime_init(void)
{
    proc_entry = proc_create(PROC_NAME,
                             0444,
                             NULL,
                             &proc_fops);

    if (!proc_entry)
        return -ENOMEM;

    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);

    return 0;
}

/* Module Exit */

static void __exit uptime_exit(void)
{
    proc_remove(proc_entry);

    printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

module_init(uptime_init);
module_exit(uptime_exit);
