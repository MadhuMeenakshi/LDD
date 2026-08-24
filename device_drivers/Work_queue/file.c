#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/workqueue.h>

#define DEVICE_NAME "mychar"
#define CLASS_NAME  "myclass"
#define BUFFER_SIZE 1024

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Madhu");
MODULE_DESCRIPTION("Assignment 11 - Workqueue Driver");
MODULE_VERSION("1.0");

static dev_t dev_num;
static struct cdev my_cdev;
static struct class *my_class;
static struct device *my_device;

static char device_buffer[BUFFER_SIZE];
static int buffer_size;

/* Workqueue */
static struct work_struct my_work;

/*-------------------------------------------------------*/
/* Work Function */
/*-------------------------------------------------------*/
static void work_handler(struct work_struct *work)
{
    printk(KERN_INFO "Workqueue Executed\n");
    printk(KERN_INFO "Buffer = %s\n", device_buffer);
}

/*-------------------------------------------------------*/
/* Open */
/*-------------------------------------------------------*/
static int my_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Device Opened\n");
    return 0;
}

/*-------------------------------------------------------*/
/* Release */
/*-------------------------------------------------------*/
static int my_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Device Closed\n");
    return 0;
}

/*-------------------------------------------------------*/
/* Read */
/*-------------------------------------------------------*/
static ssize_t my_read(struct file *file,
                       char __user *buf,
                       size_t len,
                       loff_t *off)
{
    int bytes;

    if (*off >= buffer_size)
        return 0;

    bytes = min((int)len, buffer_size - (int)(*off));

    if (copy_to_user(buf, device_buffer + *off, bytes))
        return -EFAULT;

    *off += bytes;

    return bytes;
}

/*-------------------------------------------------------*/
/* Write */
/*-------------------------------------------------------*/
static ssize_t my_write(struct file *file,
                        const char __user *buf,
                        size_t len,
                        loff_t *off)
{
    if (len >= BUFFER_SIZE)
        len = BUFFER_SIZE - 1;

    if (copy_from_user(device_buffer, buf, len))
        return -EFAULT;

    device_buffer[len] = '\0';
    buffer_size = len;

    printk(KERN_INFO "Data Received: %s\n", device_buffer);

    /* Schedule deferred work */
    schedule_work(&my_work);

    return len;
}

/*-------------------------------------------------------*/
/* File Operations */
/*-------------------------------------------------------*/
static const struct file_operations fops = {
    .owner   = THIS_MODULE,
    .open    = my_open,
    .release = my_release,
    .read    = my_read,
    .write   = my_write,
};

/*-------------------------------------------------------*/
/* Init */
/*-------------------------------------------------------*/
static int __init my_workqueue_init(void)
{
    int ret;

    ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    if (ret)
        return ret;

    cdev_init(&my_cdev, &fops);

    ret = cdev_add(&my_cdev, dev_num, 1);
    if (ret) {
        unregister_chrdev_region(dev_num, 1);
        return ret;
    }

    my_class = class_create(CLASS_NAME);
    if (IS_ERR(my_class)) {
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev_num, 1);
        return PTR_ERR(my_class);
    }

    my_device = device_create(my_class, NULL, dev_num, NULL, DEVICE_NAME);
    if (IS_ERR(my_device)) {
        class_destroy(my_class);
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev_num, 1);
        return PTR_ERR(my_device);
    }

    INIT_WORK(&my_work, work_handler);

    printk(KERN_INFO "Workqueue Driver Loaded\n");

    return 0;
}

/*-------------------------------------------------------*/
/* Exit */
/*-------------------------------------------------------*/
static void __exit my_workqueue_exit(void)
{
    flush_work(&my_work);

    device_destroy(my_class, dev_num);
    class_destroy(my_class);

    cdev_del(&my_cdev);

    unregister_chrdev_region(dev_num, 1);

    printk(KERN_INFO "Workqueue Driver Unloaded\n");
}

module_init(my_workqueue_init);
module_exit(my_workqueue_exit);
