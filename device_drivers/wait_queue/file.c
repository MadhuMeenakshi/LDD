#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/wait.h>

#define DEVICE_NAME "mychar"
#define CLASS_NAME  "myclass"
#define BUFFER_SIZE 1024

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Madhu");
MODULE_DESCRIPTION("Assignment 9 - Wait Queue");
MODULE_VERSION("1.0");

static dev_t dev_num;
static struct cdev my_cdev;
static struct class *my_class;
static struct device *my_device;

static char device_buffer[BUFFER_SIZE];
static int buffer_size;

static wait_queue_head_t my_wait_queue;
static int data_available = 0;

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

    if (wait_event_interruptible(my_wait_queue, data_available))
        return -ERESTARTSYS;

    if (*off >= buffer_size)
        return 0;

    bytes = min((int)len, buffer_size - (int)(*off));

    if (copy_to_user(buf, device_buffer + *off, bytes))
        return -EFAULT;

    *off += bytes;

    data_available = 0;

    printk(KERN_INFO "Data Read\n");

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

    data_available = 1;

    wake_up_interruptible(&my_wait_queue);

    printk(KERN_INFO "Data Written : %s\n", device_buffer);

    return len;
}

static const struct file_operations fops = {
    .owner   = THIS_MODULE,
    .open    = my_open,
    .release = my_release,
    .read    = my_read,
    .write   = my_write,
};

/* Module Init */
static int __init waitq_init(void)
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

    init_waitqueue_head(&my_wait_queue);

    printk(KERN_INFO "Wait Queue Driver Loaded\n");

    return 0;
}

/* Module Exit */
static void __exit waitq_exit(void)
{
    device_destroy(my_class, dev_num);
    class_destroy(my_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev_num, 1);

    printk(KERN_INFO "Wait Queue Driver Unloaded\n");
}

module_init(waitq_init);
module_exit(waitq_exit);
