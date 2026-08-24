#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "mychar"
#define CLASS_NAME  "myclass"
#define MAX_DATA    256

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Madhu");
MODULE_DESCRIPTION("Assignment 5 - Kernel Linked List");
MODULE_VERSION("1.0");

struct my_node {
    char data[MAX_DATA];
    struct list_head list;
};

static LIST_HEAD(data_list);

static dev_t dev_num;
static struct cdev my_cdev;
static struct class *my_class;
static struct device *my_device;

/*---------------------------------------------------------*/
/* Open */
/*---------------------------------------------------------*/
static int my_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "mychar: Device Opened\n");
    return 0;
}

/*---------------------------------------------------------*/
/* Release */
/*---------------------------------------------------------*/
static int my_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "mychar: Device Closed\n");
    return 0;
}

/*---------------------------------------------------------*/
/* Write */
/*---------------------------------------------------------*/
static ssize_t my_write(struct file *file,
                        const char __user *buf,
                        size_t len,
                        loff_t *off)
{
    struct my_node *node;

    node = kmalloc(sizeof(*node), GFP_KERNEL);
    if (!node)
        return -ENOMEM;

    if (len >= MAX_DATA)
        len = MAX_DATA - 1;

    if (copy_from_user(node->data, buf, len)) {
        kfree(node);
        return -EFAULT;
    }

    node->data[len] = '\0';

    list_add_tail(&node->list, &data_list);

    printk(KERN_INFO "Stored: %s\n", node->data);

    return len;
}

/*---------------------------------------------------------*/
/* Read */
/*---------------------------------------------------------*/
static ssize_t my_read(struct file *file,
                       char __user *buf,
                       size_t len,
                       loff_t *off)
{
    struct my_node *node;
    char temp[2048];
    int pos = 0;
    int ret;

    if (*off != 0)
        return 0;

    memset(temp, 0, sizeof(temp));

    list_for_each_entry(node, &data_list, list) {
        pos += scnprintf(temp + pos,
                         sizeof(temp) - pos,
                         "%s\n",
                         node->data);
    }

    if (pos == 0)
        return 0;

    if (len < pos)
        pos = len;

    ret = copy_to_user(buf, temp, pos);
    if (ret)
        return -EFAULT;

    *off += pos;

    return pos;
}

/*---------------------------------------------------------*/
/* File Operations */
/*---------------------------------------------------------*/
static const struct file_operations fops = {
    .owner   = THIS_MODULE,
    .open    = my_open,
    .release = my_release,
    .read    = my_read,
    .write   = my_write,
};

/*---------------------------------------------------------*/
/* Module Init */
/*---------------------------------------------------------*/
static int __init linked_list_init(void)
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

    my_device = device_create(my_class,
                              NULL,
                              dev_num,
                              NULL,
                              DEVICE_NAME);

    if (IS_ERR(my_device)) {
        class_destroy(my_class);
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev_num, 1);
        return PTR_ERR(my_device);
    }

    printk(KERN_INFO "=====================================\n");
    printk(KERN_INFO "Kernel Linked List Driver Loaded\n");
    printk(KERN_INFO "Major = %d Minor = %d\n",
           MAJOR(dev_num), MINOR(dev_num));
    printk(KERN_INFO "/dev/%s created\n", DEVICE_NAME);
    printk(KERN_INFO "=====================================\n");

    return 0;
}

/*---------------------------------------------------------*/
/* Module Exit */
/*---------------------------------------------------------*/
static void __exit linked_list_exit(void)
{
    struct my_node *node;
    struct my_node *tmp;

    list_for_each_entry_safe(node, tmp, &data_list, list) {
        list_del(&node->list);
        kfree(node);
    }

    device_destroy(my_class, dev_num);
    class_destroy(my_class);

    cdev_del(&my_cdev);

    unregister_chrdev_region(dev_num, 1);

    printk(KERN_INFO "Linked List Driver Unloaded\n");
}

module_init(linked_list_init);
module_exit(linked_list_exit);
