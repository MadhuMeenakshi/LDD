#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/gpio/consumer.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "bbb_gpio"
#define CLASS_NAME  "bbb_gpio_class"

static struct gpio_desc *led_gpio;

static dev_t dev_num;
static struct cdev gpio_cdev;
static struct class *gpio_class;
static struct device *gpio_device;


/* ---------------- OPEN ---------------- */

static int gpio_open(struct inode *inode, struct file *file)
{
    pr_info("BBB GPIO: device opened\n");

    return 0;
}


/* ---------------- WRITE ---------------- */

static ssize_t gpio_write(struct file *file,
                          const char __user *buf,
                          size_t count,
                          loff_t *ppos)
{
    char value;

    if (copy_from_user(&value, buf, 1))
        return -EFAULT;

    if (value == '1')
    {
        gpiod_set_value(led_gpio, 1);

        pr_info("BBB GPIO: LED ON\n");
    }
    else if (value == '0')
    {
        gpiod_set_value(led_gpio, 0);

        pr_info("BBB GPIO: LED OFF\n");
    }
    else
    {
        pr_info("BBB GPIO: use 1 or 0\n");

        return -EINVAL;
    }

    return 1;
}


/* ---------------- FILE OPERATIONS ---------------- */

static struct file_operations fops =
{
    .owner = THIS_MODULE,
    .open  = gpio_open,
    .write = gpio_write,
};


/* ---------------- PROBE ---------------- */

static int gpio_probe(struct platform_device *pdev)
{
    int ret;

    pr_info("BBB GPIO: probe called\n");


    /* Get GPIO from Device Tree */

    led_gpio = devm_gpiod_get(&pdev->dev,
                              "led",
                              GPIOD_OUT_LOW);

    if (IS_ERR(led_gpio))
    {
        pr_err("BBB GPIO: failed to get GPIO\n");

        return PTR_ERR(led_gpio);
    }


    /* Allocate device number */

    ret = alloc_chrdev_region(&dev_num,
                              0,
                              1,
                              DEVICE_NAME);

    if (ret < 0)
    {
        pr_err("BBB GPIO: alloc_chrdev_region failed\n");

        return ret;
    }


    /* Initialize cdev */

    cdev_init(&gpio_cdev, &fops);

    gpio_cdev.owner = THIS_MODULE;


    /* Add cdev */

    ret = cdev_add(&gpio_cdev,
                   dev_num,
                   1);

    if (ret < 0)
    {
        pr_err("BBB GPIO: cdev_add failed\n");

        goto unregister_chrdev;
    }


    /* Create class */

    gpio_class = class_create(CLASS_NAME);

    if (IS_ERR(gpio_class))
    {
        ret = PTR_ERR(gpio_class);

        goto delete_cdev;
    }


    /* Create device */

    gpio_device = device_create(gpio_class,
                                NULL,
                                dev_num,
                                NULL,
                                DEVICE_NAME);

    if (IS_ERR(gpio_device))
    {
        ret = PTR_ERR(gpio_device);

        goto destroy_class;
    }


    pr_info("BBB GPIO: driver loaded\n");

    return 0;


destroy_class:

    class_destroy(gpio_class);


delete_cdev:

    cdev_del(&gpio_cdev);


unregister_chrdev:

    unregister_chrdev_region(dev_num, 1);

    return ret;
}


/* ---------------- REMOVE ---------------- */

static int gpio_remove(struct platform_device *pdev)
{
    pr_info("BBB GPIO: remove called\n");

    gpiod_set_value(led_gpio, 0);

    device_destroy(gpio_class, dev_num);

    class_destroy(gpio_class);

    cdev_del(&gpio_cdev);

    unregister_chrdev_region(dev_num, 1);

    return 0;
}


/* ---------------- DEVICE TREE MATCH ---------------- */

static const struct of_device_id gpio_of_match[] =
{
    {
        .compatible = "mycompany,bbb-gpio"
    },

    { }
};

MODULE_DEVICE_TABLE(of, gpio_of_match);


/* ---------------- PLATFORM DRIVER ---------------- */

static struct platform_driver gpio_driver =
{
    .probe  = gpio_probe,
    .remove = gpio_remove,

    .driver =
    {
        .name = "bbb_gpio_driver",
        .of_match_table = gpio_of_match,
    },
};


/* ---------------- INIT ---------------- */

static int __init gpio_init(void)
{
    pr_info("BBB GPIO: module init\n");

    return platform_driver_register(&gpio_driver);
}


/* ---------------- EXIT ---------------- */

static void __exit gpio_exit(void)
{
    pr_info("BBB GPIO: module exit\n");

    platform_driver_unregister(&gpio_driver);
}


module_init(gpio_init);
module_exit(gpio_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Madhu");
MODULE_DESCRIPTION("BeagleBone Black GPIO Character Driver");
