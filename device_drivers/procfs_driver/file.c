#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#define DEVICE_NAME "mychar"
#define CLASS_NAME  "myclass"
#define PROC_NAME   "mydriver"
#define BUFFER_SIZE 1024

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Madhu");
MODULE_DESCRIPTION("Assignment 7 - Proc Filesystem Driver");
MODULE_VERSION("1.0");

static dev_t dev_num;
static struct cdev my_cdev;
static struct class *my_class;
static struct device *my_device;
static struct proc_dir_entry *proc_entry;

static char device_buffer[BUFFER_SIZE];
static size_t buffer_size;

static unsigned int read_count;
static unsigned int write_count;

#define DRIVER_VERSION "1.0"

/*----------------------------------------------------------*/
/* Open */
/*----------------------------------------------------------*/
static int my_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Device Opened\n");
    return 0;
}

/*----------------------------------------------------------*/
/* Release */
/*----------------------------------------------------------*/
static int my_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Device Closed\n");
    return 0;
}

/*----------------------------------------------------------*/
/* Read */
/*----------------------------------------------------------*/
static ssize_t my_read(struct file *file,
                       char __user *buf,
                       size_t len,
                       loff_t *off)
{
    size_t bytes;

    if (*off >= buffer_size)
        return 0;

    bytes = min(len, buffer_size - (size_t)(*off));

    if (copy_to_user(buf, device_buffer + *off, bytes))
        return -EFAULT;

    *off += bytes;
    read_count++;

    return bytes;
}

/*----------------------------------------------------------*/
/* Write */
/*----------------------------------------------------------*/
static ssize_t my_write(struct file *file,
                        const char __user *buf,
                        size_t len,
                        loff_t *off)
{
    size_t bytes;

    bytes = min(len, (size_t)(BUFFER_SIZE - 1));

    if (copy_from_user(device_buffer, buf, bytes))
        return -EFAULT;

    device_buffer[bytes] = '\0';
    buffer_size = bytes;
    write_count++;

    printk(KERN_INFO "Received: %s\n", device_buffer);

    return bytes;
}

/*----------------------------------------------------------*/
/* Proc Show */
/*----------------------------------------------------------*/
static int proc_show(struct seq_file *m, void *v)
{
    seq_printf(m, "Driver Version : %s\n", DRIVER_VERSION);
    seq_printf(m, "Read Count     : %u\n", read_count);
    seq_printf(m, "Write Count    : %u\n", write_count);

    return 0;
}

static int proc_open(struct inode *inode, struct file *file)
{
    return single_open(file, proc_show, NULL);
}

static const struct proc_ops proc_fops = {
    .proc_open    = proc_open,
    .proc_read    = seq_read,
    .proc_lseek   = seq_lseek,
    .proc_release = single_release,
};

/*----------------------------------------------------------*/
/* File Operations */
/*----------------------------------------------------------*/
static const struct file_operations fops = {
    .owner   = THIS_MODULE,
    .open    = my_open,
    .release = my_release,
    .read    = my_read,
    .write   = my_write,
};

/*----------------------------------------------------------*/
/* Init */
/*----------------------------------------------------------*/
static int __init proc_driver_init(void)
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

    proc_entry = proc_create(PROC_NAME,
                             0444,
                             NULL,
                             &proc_fops);

    if (!proc_entry) {
        ret = -ENOMEM;
        goto err4;
    }

    printk(KERN_INFO "Proc Driver Loaded\n");
    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);

    return 0;

err4:
    device_destroy(my_class, dev_num);
err3:
    class_destroy(my_class);
err2:
    cdev_del(&my_cdev);
err1:
    unregister_chrdev_region(dev_num, 1);

    return ret;
}

/*----------------------------------------------------------*/
/* Exit */
/*----------------------------------------------------------*/
static void __exit proc_driver_exit(void)
{
    remove_proc_entry(PROC_NAME, NULL);

    device_destroy(my_class, dev_num);
    class_destroy(my_class);

    cdev_del(&my_cdev);

    unregister_chrdev_region(dev_num, 1);

    printk(KERN_INFO "Proc Driver Unloaded\n");
}

module_init(proc_driver_init);
module_exit(proc_driver_exit);
