#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/random.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/wait.h>
#include <linux/poll.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/slab.h>
#include <linux/ioctl.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Madhu");
MODULE_DESCRIPTION("Virtual Temperature Sensor Driver");
MODULE_VERSION("1.0");

#define DEVICE_NAME "tempsensor"
#define CLASS_NAME  "tempsensor_class"
#define PROC_NAME   "tempsensor"
#define SYSFS_NAME  "tempsensor"

#define MAGIC_NUM 'T'
#define SET_HIGH_LIMIT _IOW(MAGIC_NUM,1,int)
#define SET_LOW_LIMIT  _IOW(MAGIC_NUM,2,int)
#define GET_TEMP       _IOR(MAGIC_NUM,3,int)

static dev_t dev_num;
static struct cdev temp_cdev;
static struct class *temp_class;
static struct device *temp_device;

static struct task_struct *temp_thread;
static wait_queue_head_t temp_waitqueue;
static struct mutex temp_mutex;

static struct proc_dir_entry *proc_entry;
static struct kobject *temp_kobj;

static int current_temp = 25;
static int high_limit = 45;
static int low_limit = 20;
static int sensor_enable = 1;

static unsigned long read_count;
static unsigned long threshold_events;

static char *buffer;

/*-----------------------------------------------------------*/
/* Open */
/*-----------------------------------------------------------*/
static int temp_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Temperature Sensor Opened\n");
    return 0;
}

/*-----------------------------------------------------------*/
/* Release */
/*-----------------------------------------------------------*/
static int temp_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Temperature Sensor Closed\n");
    return 0;
}

/*-----------------------------------------------------------*/
/* Read */
/*-----------------------------------------------------------*/
static ssize_t temp_read(struct file *file,
                         char __user *buf,
                         size_t len,
                         loff_t *offset)
{
    int ret;
    int bytes;
    char temp_buf[64];

    if (*offset != 0)
        return 0;

    if (wait_event_interruptible(temp_waitqueue,
                                 current_temp >= high_limit ||
                                 current_temp <= low_limit))
        return -ERESTARTSYS;

    mutex_lock(&temp_mutex);

    bytes = snprintf(temp_buf,
                     sizeof(temp_buf),
                     "Temperature : %d C\n",
                     current_temp);

    ret = copy_to_user(buf, temp_buf, bytes);

    mutex_unlock(&temp_mutex);

    if (ret)
        return -EFAULT;

    read_count++;

    *offset += bytes;

    return bytes;
}

/*-----------------------------------------------------------*/
/* Write */
/*-----------------------------------------------------------*/
static ssize_t temp_write(struct file *file,
                          const char __user *buf,
                          size_t len,
                          loff_t *offset)
{
    if (len > 100)
        len = 100;

    mutex_lock(&temp_mutex);

    if (copy_from_user(buffer, buf, len)) {
        mutex_unlock(&temp_mutex);
        return -EFAULT;
    }

    buffer[len] = '\0';

    printk(KERN_INFO "User Message : %s\n", buffer);

    mutex_unlock(&temp_mutex);

    return len;
}

/*-----------------------------------------------------------*/
/* IOCTL */
/*-----------------------------------------------------------*/
static long temp_ioctl(struct file *file,
                       unsigned int cmd,
                       unsigned long arg)
{
    int value;

    switch (cmd) {

    case SET_HIGH_LIMIT:

        if (copy_from_user(&value,
                           (int __user *)arg,
                           sizeof(value)))
            return -EFAULT;

        high_limit = value;

        printk(KERN_INFO "High Limit = %d\n",
               high_limit);

        break;

    case SET_LOW_LIMIT:

        if (copy_from_user(&value,
                           (int __user *)arg,
                           sizeof(value)))
            return -EFAULT;

        low_limit = value;

        printk(KERN_INFO "Low Limit = %d\n",
               low_limit);

        break;

    case GET_TEMP:

        value = current_temp;

        if (copy_to_user((int __user *)arg,
                         &value,
                         sizeof(value)))
            return -EFAULT;

        break;

    default:
        return -EINVAL;
    }

    return 0;
}

/*-----------------------------------------------------------*/
/* Poll */
/*-----------------------------------------------------------*/
static __poll_t temp_poll(struct file *file,
                          poll_table *wait)
{
    __poll_t mask = 0;

    poll_wait(file,
              &temp_waitqueue,
              wait);

    if (current_temp >= high_limit ||
        current_temp <= low_limit)
        mask |= POLLIN | POLLRDNORM;

    return mask;
}

/*-----------------------------------------------------------*/
/* File Operations */
/*-----------------------------------------------------------*/
static const struct file_operations temp_fops = {
    .owner = THIS_MODULE,
    .open = temp_open,
    .release = temp_release,
    .read = temp_read,
    .write = temp_write,
    .unlocked_ioctl = temp_ioctl,
    .poll = temp_poll,
};

/*-----------------------------------------------------------*/
/* Kernel Thread */
/*-----------------------------------------------------------*/
static int temperature_thread(void *data)
{
    u32 random;

    while (!kthread_should_stop()) {

        if (sensor_enable) {

            get_random_bytes(&random, sizeof(random));

            mutex_lock(&temp_mutex);

            current_temp = (random % 51) + 10;   /* 10°C to 60°C */

            printk(KERN_INFO
                   "Current Temperature : %d C\n",
                   current_temp);

            if (current_temp >= high_limit ||
                current_temp <= low_limit) {

                threshold_events++;

                wake_up_interruptible(&temp_waitqueue);
            }

            mutex_unlock(&temp_mutex);
        }

        ssleep(3);
    }

    printk(KERN_INFO "Temperature Thread Stopped\n");

    return 0;
}

/*-----------------------------------------------------------*/
/* Procfs */
/*-----------------------------------------------------------*/

static int proc_show(struct seq_file *m, void *v)
{
    seq_printf(m, "Current Temperature : %d C\n",
               current_temp);

    seq_printf(m, "High Limit         : %d C\n",
               high_limit);

    seq_printf(m, "Low Limit          : %d C\n",
               low_limit);

    seq_printf(m, "Read Count         : %lu\n",
               read_count);

    seq_printf(m, "Threshold Events   : %lu\n",
               threshold_events);

    return 0;
}

static int proc_open(struct inode *inode,
                     struct file *file)
{
    return single_open(file,
                       proc_show,
                       NULL);
}

static const struct proc_ops proc_fops = {
    .proc_open    = proc_open,
    .proc_read    = seq_read,
    .proc_lseek   = seq_lseek,
    .proc_release = single_release,
};

/*-----------------------------------------------------------*/
/* Sysfs */
/*-----------------------------------------------------------*/

static ssize_t enable_show(struct kobject *kobj,
                           struct kobj_attribute *attr,
                           char *buf)
{
    return sprintf(buf, "%d\n", sensor_enable);
}

static ssize_t enable_store(struct kobject *kobj,
                            struct kobj_attribute *attr,
                            const char *buf,
                            size_t count)
{
    kstrtoint(buf, 10, &sensor_enable);

    return count;
}

static struct kobj_attribute enable_attr =
        __ATTR(enable,
               0664,
               enable_show,
               enable_store);

/*-----------------------------------------------------------*/

static ssize_t high_show(struct kobject *kobj,
                         struct kobj_attribute *attr,
                         char *buf)
{
    return sprintf(buf,
                   "%d\n",
                   high_limit);
}

static ssize_t high_store(struct kobject *kobj,
                          struct kobj_attribute *attr,
                          const char *buf,
                          size_t count)
{
    kstrtoint(buf,
              10,
              &high_limit);

    return count;
}

static struct kobj_attribute high_attr =
        __ATTR(high_limit,
               0664,
               high_show,
               high_store);

/*-----------------------------------------------------------*/

static ssize_t low_show(struct kobject *kobj,
                        struct kobj_attribute *attr,
                        char *buf)
{
    return sprintf(buf,
                   "%d\n",
                   low_limit);
}

static ssize_t low_store(struct kobject *kobj,
                         struct kobj_attribute *attr,
                         const char *buf,
                         size_t count)
{
    kstrtoint(buf,
              10,
              &low_limit);

    return count;
}

static struct kobj_attribute low_attr =
        __ATTR(low_limit,
               0664,
               low_show,
               low_store);
               
               /*-----------------------------------------------------------*/
/* Module Init */
/*-----------------------------------------------------------*/

static int __init temp_driver_init(void)
{
    int ret;

    buffer = kmalloc(128, GFP_KERNEL);
    if (!buffer)
        return -ENOMEM;

    mutex_init(&temp_mutex);

    init_waitqueue_head(&temp_waitqueue);

    ret = alloc_chrdev_region(&dev_num,
                              0,
                              1,
                              DEVICE_NAME);
    if (ret)
        goto err_mem;

    cdev_init(&temp_cdev,
              &temp_fops);

    temp_cdev.owner = THIS_MODULE;

    ret = cdev_add(&temp_cdev,
                   dev_num,
                   1);

    if (ret)
        goto err_chrdev;

    temp_class = class_create(CLASS_NAME);

    if (IS_ERR(temp_class)) {
        ret = PTR_ERR(temp_class);
        goto err_cdev;
    }

    temp_device = device_create(temp_class,
                                NULL,
                                dev_num,
                                NULL,
                                DEVICE_NAME);

    if (IS_ERR(temp_device)) {
        ret = PTR_ERR(temp_device);
        goto err_class;
    }

    proc_entry = proc_create(PROC_NAME,
                             0444,
                             NULL,
                             &proc_fops);

    if (!proc_entry) {
        ret = -ENOMEM;
        goto err_device;
    }

    temp_kobj = kobject_create_and_add(SYSFS_NAME,
                                       kernel_kobj);

    if (!temp_kobj) {
        ret = -ENOMEM;
        goto err_proc;
    }

    sysfs_create_file(temp_kobj,
                      &enable_attr.attr);

    sysfs_create_file(temp_kobj,
                      &high_attr.attr);

    sysfs_create_file(temp_kobj,
                      &low_attr.attr);

    temp_thread = kthread_run(temperature_thread,
                              NULL,
                              "temp_thread");

    if (IS_ERR(temp_thread)) {
        ret = PTR_ERR(temp_thread);
        goto err_sysfs;
    }

    printk(KERN_INFO "====================================\n");
    printk(KERN_INFO "Virtual Temperature Sensor Loaded\n");
    printk(KERN_INFO "Major Number : %d\n", MAJOR(dev_num));
    printk(KERN_INFO "Device : /dev/%s\n", DEVICE_NAME);
    printk(KERN_INFO "Procfs : /proc/%s\n", PROC_NAME);
    printk(KERN_INFO "Sysfs : /sys/kernel/%s\n", SYSFS_NAME);
    printk(KERN_INFO "====================================\n");

    return 0;

err_sysfs:
    sysfs_remove_file(temp_kobj, &enable_attr.attr);
    sysfs_remove_file(temp_kobj, &high_attr.attr);
    sysfs_remove_file(temp_kobj, &low_attr.attr);
    kobject_put(temp_kobj);

err_proc:
    proc_remove(proc_entry);

err_device:
    device_destroy(temp_class,
                   dev_num);

err_class:
    class_destroy(temp_class);

err_cdev:
    cdev_del(&temp_cdev);

err_chrdev:
    unregister_chrdev_region(dev_num,
                             1);

err_mem:
    kfree(buffer);

    return ret;
}

/*-----------------------------------------------------------*/
/* Module Exit */
/*-----------------------------------------------------------*/

static void __exit temp_driver_exit(void)
{
    if (temp_thread)
        kthread_stop(temp_thread);

    sysfs_remove_file(temp_kobj,
                      &enable_attr.attr);

    sysfs_remove_file(temp_kobj,
                      &high_attr.attr);

    sysfs_remove_file(temp_kobj,
                      &low_attr.attr);

    kobject_put(temp_kobj);

    proc_remove(proc_entry);

    device_destroy(temp_class,
                   dev_num);

    class_destroy(temp_class);

    cdev_del(&temp_cdev);

    unregister_chrdev_region(dev_num,
                             1);

    kfree(buffer);

    printk(KERN_INFO
           "Virtual Temperature Sensor Unloaded\n");
}

module_init(temp_driver_init);
module_exit(temp_driver_exit);
