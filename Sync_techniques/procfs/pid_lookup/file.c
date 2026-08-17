#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/sched/signal.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Madhu");
MODULE_DESCRIPTION("PID Lookup using procfs");

#define PROC_NAME "pidinfo"
#define BUFFER_SIZE 64

static struct proc_dir_entry *proc_entry;
static pid_t pid = -1;

/* Read */

static ssize_t proc_read(struct file *file,
                         char __user *buffer,
                         size_t count,
                         loff_t *ppos)
{
    char output[256];
    int len;
    struct task_struct *task;

    task = pid_task(find_vpid(pid), PIDTYPE_PID);

    if (!task)
    {
        len = snprintf(output,
                       sizeof(output),
                       "PID %d not found\n",
                       pid);
    }
    else
    {
        len = snprintf(output,
                       sizeof(output),
                       "PID         : %d\n"
                       "Name        : %s\n"
                       "Parent PID  : %d\n"
                       "State       : %u\n",
                       task->pid,
                       task->comm,
                       task->real_parent->pid,
                       task->__state);
    }

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
    char input[BUFFER_SIZE];

    if (count >= BUFFER_SIZE)
        count = BUFFER_SIZE - 1;

    if (copy_from_user(input, buffer, count))
        return -EFAULT;

    input[count] = '\0';

    if (kstrtoint(input, 10, &pid))
        return -EINVAL;

    printk(KERN_INFO "PID Received : %d\n", pid);

    return count;
}

/* Operations */

static const struct proc_ops proc_fops =
{
    .proc_read  = proc_read,
    .proc_write = proc_write,
};

/* Init */

static int __init pidinfo_init(void)
{
    proc_entry = proc_create(PROC_NAME,
                             0666,
                             NULL,
                             &proc_fops);

    if (!proc_entry)
        return -ENOMEM;

    printk(KERN_INFO "/proc/pidinfo created\n");

    return 0;
}

/* Exit */

static void __exit pidinfo_exit(void)
{
    proc_remove(proc_entry);

    printk(KERN_INFO "/proc/pidinfo removed\n");
}

module_init(pidinfo_init);
module_exit(pidinfo_exit);
