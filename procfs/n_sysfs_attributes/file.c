#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Madhu");
MODULE_DESCRIPTION("LED Simulation using sysfs");

static struct kobject *my_kobj;
static int brightness = 0;

/* Read Function */
static ssize_t brightness_show(struct kobject *kobj,
                               struct kobj_attribute *attr,
                               char *buf)
{
    return sprintf(buf, "%d\n", brightness);
}

/* Write Function */
static ssize_t brightness_store(struct kobject *kobj,
                                struct kobj_attribute *attr,
                                const char *buf,
                                size_t count)
{
    int value;

    if (kstrtoint(buf, 10, &value))
        return -EINVAL;

    if (value == 0)
    {
        brightness = 0;
        printk(KERN_INFO "LED OFF\n");
    }
    else if (value == 1)
    {
        brightness = 1;
        printk(KERN_INFO "LED ON\n");
    }
    else
    {
        printk(KERN_INFO "Invalid Value\n");
        return -EINVAL;
    }

    return count;
}

/* Attribute */
static struct kobj_attribute brightness_attribute =
        __ATTR(brightness, 0664,
               brightness_show,
               brightness_store);

/* Module Init */
static int __init led_sysfs_init(void)
{
    int ret;

    my_kobj = kobject_create_and_add("mydriver", kernel_kobj);

    if (!my_kobj)
        return -ENOMEM;

    ret = sysfs_create_file(my_kobj,
                            &brightness_attribute.attr);

    if (ret)
    {
        kobject_put(my_kobj);
        return ret;
    }

    printk(KERN_INFO "LED sysfs Created\n");

    return 0;
}

/* Module Exit */
static void __exit led_sysfs_exit(void)
{
    sysfs_remove_file(my_kobj,
                      &brightness_attribute.attr);

    kobject_put(my_kobj);

    printk(KERN_INFO "LED sysfs Removed\n");
}

module_init(led_sysfs_init);
module_exit(led_sysfs_exit);
