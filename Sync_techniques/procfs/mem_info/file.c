#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/mm.h>
#include <linux/sysinfo.h>

#define PROC_NAME "memory_info"

static struct proc_dir_entry *proc_entry;

/* Read Function */
static ssize_t proc_read(struct file *file,
                         char __user *buffer,
                         size_t count,
                         loff_t *ppos)
{
    char message[256];
    int len;
    struct sysinfo info;

    si_meminfo(&info);

    len = snprintf(message,
                   sizeof(message),
                   "Total RAM   : %lu KB\n"
                   "Free RAM    : %lu KB\n"
                   "Buffer RAM  : %lu KB\n"
                   "Shared RAM  : %lu KB\n",
                   (info.totalram * info.mem_unit) / 1024,
                   (info.freeram * info.mem_unit) / 1024,
                   (info.bufferram * info.mem_unit) / 1024,
                   (info.sharedram * info.mem_unit) / 1024);

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
static int __init memory_init(void)
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
static void __exit memory_exit(void)
{
    proc_remove(proc_entry);

    printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

module_init(memory_init);
module_exit(memory_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Madhu");
MODULE_DESCRIPTION("Memory Information using procfs");
