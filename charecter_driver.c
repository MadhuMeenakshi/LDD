#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "mydevice"
#define CLASS_NAME  "myclass"

static dev_t dev_num;
static struct cdev my_cdev;
static struct class *my_class;
static struct device *my_device;

static char device_buffer[100];

/* ---------------- OPEN ---------------- */

static int my_open(struct inode *inode, struct file *file)
{
    pr_info("mydevice: open\n");

    return 0;
}

/* ---------------- READ ---------------- */

static ssize_t my_read(struct file *file,
                        char __user *buf,
                        size_t count,
                        loff_t *offset)
{
    size_t len;

    len = strlen(device_buffer);

    if (*offset >= len)
        return 0;

    if (count > len - *offset)
        count = len - *offset;

    if (copy_to_user(buf,
                     device_buffer + *offset,
                     count))
    {
        return -EFAULT;
    }

    *offset += count;

    pr_info("mydevice: read %zu bytes\n", count);

    return count;
}

/* ---------------- WRITE ---------------- */

static ssize_t my_write(struct file *file,
                         const char __user *buf,
                         size_t count,
                         loff_t *offset)
{
    if (count >= sizeof(device_buffer))
        count = sizeof(device_buffer) - 1;

    if (copy_from_user(device_buffer,
                       buf,
                       count))
    {
        return -EFAULT;
    }

    device_buffer[count] = '\0';

    pr_info("mydevice: received: %s\n",
            device_buffer);

    return count;
}

/* ---------------- CLOSE ---------------- */

static int my_release(struct inode *inode,
                       struct file *file)
{
    pr_info("mydevice: close\n");

    return 0;
}


/* ---------------- FOPS ---------------- */

static const struct file_operations fops = {

    .owner   = THIS_MODULE,

    .open    = my_open,
    .read    = my_read,
    .write   = my_write,
    .release = my_release,
};


/* ---------------- INIT ---------------- */

static int __init my_driver_init(void)
{
    int ret;

    pr_info("mydevice: initializing\n");

    /* 1. Allocate major/minor number */

    ret = alloc_chrdev_region(&dev_num,
                              0,
                              1,
                              DEVICE_NAME);

    if (ret < 0) {
        pr_err("Failed to allocate device number\n");
        return ret;
    }

    pr_info("Major = %d\n", MAJOR(dev_num));
    pr_info("Minor = %d\n", MINOR(dev_num));


    /* 2. Initialize cdev */

    cdev_init(&my_cdev, &fops);

    my_cdev.owner = THIS_MODULE;


    /* 3. Add cdev to kernel */

    ret = cdev_add(&my_cdev,
                   dev_num,
                   1);

    if (ret < 0) {
        pr_err("Failed to add cdev\n");
        goto cleanup_chrdev;
    }


    /* 4. Create class */

    my_class = class_create(THIS_MODULE,
                            CLASS_NAME);

    if (IS_ERR(my_class)) {

        ret = PTR_ERR(my_class);

        pr_err("Failed to create class\n");

        goto cleanup_cdev;
    }


    /* 5. Create device */

    my_device = device_create(my_class,
                              NULL,
                              dev_num,
                              NULL,
                              DEVICE_NAME);

    if (IS_ERR(my_device)) {

        ret = PTR_ERR(my_device);

        pr_err("Failed to create device\n");

        goto cleanup_class;
    }

    pr_info("mydevice: driver loaded\n");

    return 0;


/* ---------------- ERROR CLEANUP ---------------- */

cleanup_class:

    class_destroy(my_class);

cleanup_cdev:

    cdev_del(&my_cdev);

cleanup_chrdev:

    unregister_chrdev_region(dev_num, 1);

    return ret;
}


/* ---------------- EXIT ---------------- */

static void __exit my_driver_exit(void)
{
    device_destroy(my_class,
                   dev_num);

    class_destroy(my_class);

    cdev_del(&my_cdev);

    unregister_chrdev_region(dev_num, 1);

    pr_info("mydevice: driver unloaded\n");
}


module_init(my_driver_init);
module_exit(my_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Madhu");
MODULE_DESCRIPTION("Basic Character Device Driver");
