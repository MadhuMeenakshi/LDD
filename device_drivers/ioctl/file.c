#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>

#define DEVICE_NAME "mychar"
#define CLASS_NAME  "myclass"
#define BUFFER_SIZE 1024

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Madhu");
MODULE_DESCRIPTION("Assignment 6 - IOCTL Driver");
MODULE_VERSION("1.0");

/* IOCTL Commands */
#define MAGIC_NUM 'M'

#define IOCTL_RESET_BUFFER   _IO(MAGIC_NUM, 0)
#define IOCTL_GET_VERSION    _IOR(MAGIC_NUM, 1, int)
#define IOCTL_SET_LED        _IOW(MAGIC_NUM, 2, int)

static dev_t dev_num;
static struct cdev my_cdev;
static struct class *my_class;
static struct device *my_device;

static char device_buffer[BUFFER_SIZE];
static int buffer_size;
static int led_status = 0;
static int driver_version = 1;

/* Open */
static int my_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Device Opened\n");
    return 0;
}

/* Release */
static int my_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Device Closed\n");
    return 0;
}

/* Read */
static ssize_t my_read(struct file *file,
                       char __user *buf,
                       size_t len,
                       loff_t *off)
{
    int bytes;

    if (*off >= buffer_size)
        return 0;

    bytes = min((int)len, buffer_size - (int)*off);

    if (copy_to_user(buf, device_buffer + *off, bytes))
        return -EFAULT;

    *off += bytes;

    return bytes;
}

/* Write */
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

    printk(KERN_INFO "Received: %s\n", device_buffer);

    return len;
}

/* IOCTL */
static long my_ioctl(struct file *file,
                     unsigned int cmd,
                     unsigned long arg)
{
    int value;

    switch (cmd) {

    case IOCTL_RESET_BUFFER:

        memset(device_buffer, 0, BUFFER_SIZE);
        buffer_size = 0;

        printk(KERN_INFO "Buffer Reset\n");
        break;

    case IOCTL_GET_VERSION:

        value = driver_version;

        if (copy_to_user((int __user *)arg,
                         &value,
                         sizeof(value)))
            return -EFAULT;

        printk(KERN_INFO "Driver Version Sent\n");
        break;

    case IOCTL_SET_LED:

        if (copy_from_user(&value,
                           (int __user *)arg,
                           sizeof(value)))
            return -EFAULT;

        led_status = value;

        printk(KERN_INFO "Dummy LED Status = %d\n",
               led_status);

        break;

    default:
        return -EINVAL;
    }

    return 0;
}

/* File Operations */
static const struct file_operations fops = {
    .owner          = THIS_MODULE,
    .open           = my_open,
    .release        = my_release,
    .read           = my_read,
    .write          = my_write,
    .unlocked_ioctl = my_ioctl,
};

/* Init */
static int __init ioctl_init(void)
{
    int ret;

    ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    if (ret)
        return ret;

    cdev_init(&my_cdev, &fops);

    ret = cdev_add(&my_cdev, dev_num, 1);
    if (ret)
        goto err1;

    my_class = class_create(CLASS_NAME);
    if (IS_ERR(my_class)) {
        ret = PTR_ERR(my_class);
        goto err2;
    }

    my_device = device_create(my_class,
                              NULL,
                              dev_num,
                              NULL,
                              DEVICE_NAME);

    if (IS_ERR(my_device)) {
        ret = PTR_ERR(my_device);
        goto err3;
    }

    printk(KERN_INFO "IOCTL Driver Loaded\n");
    printk(KERN_INFO "Major=%d Minor=%d\n",
           MAJOR(dev_num),
           MINOR(dev_num));

    return 0;

err3:
    class_destroy(my_class);
err2:
    cdev_del(&my_cdev);
err1:
    unregister_chrdev_region(dev_num,1);
    return ret;
}

/* Exit */
static void __exit ioctl_exit(void)
{
    device_destroy(my_class, dev_num);
    class_destroy(my_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev_num,1);

    printk(KERN_INFO "IOCTL Driver Unloaded\n");
}

module_init(ioctl_init);
module_exit(ioctl_exit);
