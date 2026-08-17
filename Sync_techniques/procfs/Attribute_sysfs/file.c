#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Madhu");
MODULE_DESCRIPTION("Create sysfs Attribute");

static struct kobject *my_kobj;
static int value = 100;

/* Show Function */
static ssize_t value_show(struct kobject *kobj,
                          struct kobj_attribute *attr,
                          char *buf)
{
    return sprintf(buf, "%d\n", value);
}

/* Store Function */
static ssize_t value_store(struct kobject *kobj,
                           struct kobj_attribute *attr,
                           const char *buf,
                           size_t count)
{
    sscanf(buf, "%d", &value);

    printk(KERN_INFO "New Value = %d\n", value);

    return count;
}

/* Create Attribute */
static struct kobj_attribute value_attribute =
        __ATTR(value, 0664, value_show, value_store);

/* Module Init */
static int __init my_sysfs_init(void)
{
    int ret;

    my_kobj = kobject_create_and_add("mydriver", kernel_kobj);

    if (!my_kobj)
    {
        printk(KERN_ERR "Failed to create kobject\n");
        return -ENOMEM;
    }

    ret = sysfs_create_file(my_kobj, &value_attribute.attr);

    if (ret)
    {
        printk(KERN_ERR "Failed to create sysfs file\n");
        kobject_put(my_kobj);
        return ret;
    }

    printk(KERN_INFO "/sys/kernel/mydriver/value created\n");

    return 0;
}

/* Module Exit */
static void __exit my_sysfs_exit(void)
{
    sysfs_remove_file(my_kobj, &value_attribute.attr);

    kobject_put(my_kobj);

    printk(KERN_INFO "/sys/kernel/mydriver/value removed\n");
}

module_init(my_sysfs_init);
module_exit(my_sysfs_exit);

MODULE_LICENSE("GPL");
