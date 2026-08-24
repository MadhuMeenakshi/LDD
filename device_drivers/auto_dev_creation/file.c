#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "mychar"
#define CLASS_NAME  "myclass"
#define BUFFER_SIZE 1024

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Madhu");
MODULE_DESCRIPTION("Assignment 3 - Automatic Device Creation");
MODULE_VERSION("1.0");

static dev_t dev_num;
static struct cdev my_cdev;
static struct class *my_class;
static struct device *my_device;

static char device_buffer[BUFFER_SIZE];
static size_t buffer_size;

/* Open */
static int my_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "mychar: Device Opened\n");
    return 0;
}

/* Release */
static int my_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "mychar: Device Closed\n");
    return 0;
}

/* Read */
static ssize_t my_read(struct file *file,
                       char __user *buf,
                       size_t len,
                       loff_t *offset)
{
    size_t bytes;

    if (*offset >= buffer_size)
        return 0;

    bytes = min(len, buffer_size - (size_t)(*offset));

    if (copy_to_user(buf, device_buffer + *offset, bytes))
        return -EFAULT;

    *offset += bytes;

    return bytes;
}

/* Write */
static ssize_t my_write(struct file *file,
                        const char __user *buf,
                        size_t len,
                        loff_t *offset)
{
    size_t bytes;

    bytes = min(len, (size_t)(BUFFER_SIZE - 1));

    if (copy_from_user(device_buffer, buf, bytes))
        return -EFAULT;

    device_buffer[bytes] = '\0';
    buffer_size = bytes;

    printk(KERN_INFO "mychar: Data Received: %s\n", device_buffer);

    return bytes;
}

/* File Operations */
static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_release,
    .read = my_read,
    .write = my_write,
};

/* Module Init */
static int __init my_driver_init(void)
{
    int ret;

    ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    if (ret < 0)
        return ret;

    cdev_init(&my_cdev, &fops);
    my_cdev.owner = THIS_MODULE;

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

    printk(KERN_INFO "====================================\n");
    printk(KERN_INFO "Driver Loaded Successfully\n");
    printk(KERN_INFO "Major = %d Minor = %d\n",
           MAJOR(dev_num), MINOR(dev_num));
    printk(KERN_INFO "/dev/%s created automatically\n", DEVICE_NAME);
    printk(KERN_INFO "====================================\n");

    return 0;
}

/* Module Exit */
static void __exit my_driver_exit(void)
{
    device_destroy(my_class, dev_num);
    class_destroy(my_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev_num, 1);

    printk(KERN_INFO "Driver Unloaded Successfully\n");
}

module_init(my_driver_init);
module_exit(my_driver_exit);
