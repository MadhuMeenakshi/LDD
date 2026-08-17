#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Madhu");
MODULE_DESCRIPTION("Mini Device Driver");

/* Module Parameter */

static int value = 100;

module_param(value, int, 0644);
MODULE_PARM_DESC(value, "Driver Value");

/* procfs */

#define PROC_NAME "mydriver"

static struct proc_dir_entry *proc_entry;

/* sysfs */

static struct kobject *driver_kobj;

/* ---------------- PROC READ ---------------- */

static ssize_t proc_read(struct file *file,
                         char __user *buffer,
                         size_t count,
                         loff_t *ppos)
{
    char msg[100];
    int len;

    len = snprintf(msg,
                   sizeof(msg),
                   "Driver Value : %d\n",
                   value);

    return simple_read_from_buffer(buffer,
                                   count,
                                   ppos,
                                   msg,
                                   len);
}

/* ---------------- PROC WRITE ---------------- */

static ssize_t proc_write(struct file *file,
                          const char __user *buffer,
                          size_t count,
                          loff_t *ppos)
{
    char temp[32];

    if (count >= sizeof(temp))
        count = sizeof(temp) - 1;

    if (copy_from_user(temp, buffer, count))
        return -EFAULT;

    temp[count] = '\0';

    if (kstrtoint(temp, 10, &value))
        return -EINVAL;

    printk(KERN_INFO "procfs Updated Value = %d\n", value);

    return count;
}

/* proc operations */

static const struct proc_ops proc_fops =
{
    .proc_read = proc_read,
    .proc_write = proc_write,
};

/* ---------------- SYSFS SHOW ---------------- */

static ssize_t value_show(struct kobject *kobj,
                          struct kobj_attribute *attr,
                          char *buf)
{
    return sprintf(buf, "%d\n", value);
}

/* ---------------- SYSFS STORE ---------------- */

static ssize_t value_store(struct kobject *kobj,
                           struct kobj_attribute *attr,
                           const char *buf,
                           size_t count)
{
    if (kstrtoint(buf, 10, &value))
        return -EINVAL;

    printk(KERN_INFO "sysfs Updated Value = %d\n", value);

    return count;
}

static struct kobj_attribute value_attribute =
        __ATTR(value,
               0664,
               value_show,
               value_store);

/* ---------------- INIT ---------------- */

static int __init mini_driver_init(void)
{
    int ret;

    proc_entry = proc_create(PROC_NAME,
                             0666,
                             NULL,
                             &proc_fops);

    if (!proc_entry)
        return -ENOMEM;

    driver_kobj = kobject_create_and_add("mydriver",
                                         kernel_kobj);

    if (!driver_kobj)
    {
        proc_remove(proc_entry);
        return -ENOMEM;
    }

    ret = sysfs_create_file(driver_kobj,
                            &value_attribute.attr);

    if (ret)
    {
        kobject_put(driver_kobj);
        proc_remove(proc_entry);
        return ret;
    }

    printk(KERN_INFO "Mini Driver Loaded\n");

    return 0;
}

/* ---------------- EXIT ---------------- */

static void __exit mini_driver_exit(void)
{
    sysfs_remove_file(driver_kobj,
                      &value_attribute.attr);

    kobject_put(driver_kobj);

    proc_remove(proc_entry);

    printk(KERN_INFO "Mini Driver Removed\n");
}

module_init(mini_driver_init);
module_exit(mini_driver_exit);
