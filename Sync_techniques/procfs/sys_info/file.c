#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/utsname.h>
#include <linux/mm.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Madhu");
MODULE_DESCRIPTION("System Information using procfs");

#define PROC_NAME "systeminfo"

static struct proc_dir_entry *proc_entry;

/* Read Function */

static ssize_t proc_read(struct file *file,
                         char __user *buffer,
                         size_t count,
                         loff_t *ppos)
{
    char message[512];
    int len;

    len = snprintf(message,
                   sizeof(message),
                   "CPU Architecture : %s\n"
                   "Kernel Version   : %s\n"
                   "Hostname         : %s\n"
                   "Page Size        : %lu Bytes\n",
                   init_uts_ns.name.machine,
                   init_uts_ns.name.release,
                   init_uts_ns.name.nodename,
                   PAGE_SIZE);

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

static int __init systeminfo_init(void)
{
    proc_entry = proc_create(PROC_NAME,
                             0444,
                             NULL,
                             &proc_fops);

    if (!proc_entry)
        return -ENOMEM;

    printk(KERN_INFO "/proc/systeminfo created\n");

    return 0;
}

/* Module Exit */

static void __exit systeminfo_exit(void)
{
    proc_remove(proc_entry);

    printk(KERN_INFO "/proc/systeminfo removed\n");
}

module_init(systeminfo_init);
module_exit(systeminfo_exit);
