#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Madhu");
MODULE_DESCRIPTION("Procfs Calculator");

#define PROC_NAME "calculator"
#define BUFFER_SIZE 100

static struct proc_dir_entry *proc_entry;

static char input[BUFFER_SIZE];
static int num1, num2;
static int sum;

/* Read */

static ssize_t proc_read(struct file *file,
                         char __user *buffer,
                         size_t count,
                         loff_t *ppos)
{
    char output[100];
    int len;

    len = snprintf(output,
                   sizeof(output),
                   "Sum = %d\n",
                   sum);

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

    if (copy_from_user(input, buffer, count))
        return -EFAULT;

    input[count] = '\0';

    if (sscanf(input, "%d %d", &num1, &num2) != 2)
        return -EINVAL;

    sum = num1 + num2;

    printk(KERN_INFO "%d + %d = %d\n",
           num1,
           num2,
           sum);

    return count;
}

/* Operations */

static const struct proc_ops proc_fops =
{
    .proc_read = proc_read,
    .proc_write = proc_write,
};

/* Init */

static int __init calculator_init(void)
{
    proc_entry = proc_create(PROC_NAME,
                             0666,
                             NULL,
                             &proc_fops);

    if (!proc_entry)
        return -ENOMEM;

    printk(KERN_INFO "/proc/calculator created\n");

    return 0;
}

/* Exit */

static void __exit calculator_exit(void)
{
    proc_remove(proc_entry);

    printk(KERN_INFO "/proc/calculator removed\n");
}

module_init(calculator_init);
module_exit(calculator_exit);
