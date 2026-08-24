#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "mychar"
#define BUFFER_SIZE 1024

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Madhu");
MODULE_DESCRIPTION("Assignment 1 - Character Device Driver");
MODULE_VERSION("1.0");

static int major_number;
static char device_buffer[BUFFER_SIZE];
static size_t buffer_size = 0;

/* Open */
static int my_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "mychar: Device opened\n");
    return 0;
}

/* Release */
static int my_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "mychar: Device closed\n");
    return 0;
}

/* Read */
static ssize_t my_read(struct file *file,
                       char __user *user_buffer,
                       size_t len,
                       loff_t *offset)
{
    size_t bytes_to_read;

    if (*offset >= buffer_size)
        return 0;

    bytes_to_read = min(len, buffer_size - (size_t)(*offset));

    if (copy_to_user(user_buffer,
                     device_buffer + *offset,
                     bytes_to_read))
        return -EFAULT;

    *offset += bytes_to_read;

    printk(KERN_INFO "mychar: Read %zu bytes\n", bytes_to_read);

    return bytes_to_read;
}

/* Write */
static ssize_t my_write(struct file *file,
                        const char __user *user_buffer,
                        size_t len,
                        loff_t *offset)
{
    size_t bytes_to_write;

    bytes_to_write = min(len, (size_t)(BUFFER_SIZE - 1));

    if (copy_from_user(device_buffer,
                       user_buffer,
                       bytes_to_write))
        return -EFAULT;

    device_buffer[bytes_to_write] = '\0';
    buffer_size = bytes_to_write;

    printk(KERN_INFO "mychar: Received %zu bytes\n", bytes_to_write);
    printk(KERN_INFO "mychar: Data = %s\n", device_buffer);

    return bytes_to_write;
}

/* File operations */
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_release,
    .read = my_read,
    .write = my_write,
};

/* Module Init */
static int __init mychar_init(void)
{
    major_number = register_chrdev(0, DEVICE_NAME, &fops);

    if (major_number < 0) {
        printk(KERN_ALERT "mychar: Failed to register device\n");
        return major_number;
    }

    printk(KERN_INFO "=====================================\n");
    printk(KERN_INFO "mychar: Driver Loaded Successfully\n");
    printk(KERN_INFO "Major Number = %d\n", major_number);
    printk(KERN_INFO "Create device using:\n");
    printk(KERN_INFO "mknod /dev/mychar c %d 0\n", major_number);
    printk(KERN_INFO "chmod 666 /dev/mychar\n");
    printk(KERN_INFO "=====================================\n");

    return 0;
}

/* Module Exit */
static void __exit mychar_exit(void)
{
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "mychar: Driver Unloaded\n");
}

module_init(mychar_init);
module_exit(mychar_exit);
