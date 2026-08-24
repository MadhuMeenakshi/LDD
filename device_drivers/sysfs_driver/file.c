#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Madhu");
MODULE_DESCRIPTION("Assignment 8 - Sysfs Interface");
MODULE_VERSION("1.0");

static struct kobject *my_kobj;

static int enable = 0;
static int status = 1;
static int buffer_size = 1024;

/*------------------------------------------------*/
/* enable attribute */
/*------------------------------------------------*/

static ssize_t enable_show(struct kobject *kobj,
                           struct kobj_attribute *attr,
                           char *buf)
{
    return sprintf(buf, "%d\n", enable);
}

static ssize_t enable_store(struct kobject *kobj,
                            struct kobj_attribute *attr,
                            const char *buf,
                            size_t count)
{
    sscanf(buf, "%d", &enable);

    printk(KERN_INFO "enable = %d\n", enable);

    return count;
}

static struct kobj_attribute enable_attr =
        __ATTR(enable, 0664, enable_show, enable_store);

/*------------------------------------------------*/
/* status attribute */
/*------------------------------------------------*/

static ssize_t status_show(struct kobject *kobj,
                           struct kobj_attribute *attr,
                           char *buf)
{
    return sprintf(buf, "%d\n", status);
}

static struct kobj_attribute status_attr =
        __ATTR(status, 0444, status_show, NULL);

/*------------------------------------------------*/
/* buffer_size attribute */
/*------------------------------------------------*/

static ssize_t buffer_size_show(struct kobject *kobj,
                                struct kobj_attribute *attr,
                                char *buf)
{
    return sprintf(buf, "%d\n", buffer_size);
}

static ssize_t buffer_size_store(struct kobject *kobj,
                                 struct kobj_attribute *attr,
                                 const char *buf,
                                 size_t count)
{
    sscanf(buf, "%d", &buffer_size);

    printk(KERN_INFO "buffer_size = %d\n",
            buffer_size);

    return count;
}

static struct kobj_attribute buffer_size_attr =
        __ATTR(buffer_size,
               0664,
               buffer_size_show,
               buffer_size_store);

/*------------------------------------------------*/
/* Init */
/*------------------------------------------------*/

static int __init sysfs_driver_init(void)
{
    int ret;

    my_kobj = kobject_create_and_add("mydriver",
                                     kernel_kobj);

    if (!my_kobj)
        return -ENOMEM;

    ret = sysfs_create_file(my_kobj,
                            &enable_attr.attr);
    if (ret)
        goto err;

    ret = sysfs_create_file(my_kobj,
                            &status_attr.attr);
    if (ret)
        goto err;

    ret = sysfs_create_file(my_kobj,
                            &buffer_size_attr.attr);
    if (ret)
        goto err;

    printk(KERN_INFO "Sysfs Driver Loaded\n");

    return 0;

err:
    kobject_put(my_kobj);
    return ret;
}

/*------------------------------------------------*/
/* Exit */
/*------------------------------------------------*/

static void __exit sysfs_driver_exit(void)
{
    sysfs_remove_file(my_kobj,
                      &enable_attr.attr);

    sysfs_remove_file(my_kobj,
                      &status_attr.attr);

    sysfs_remove_file(my_kobj,
                      &buffer_size_attr.attr);

    kobject_put(my_kobj);

    printk(KERN_INFO "Sysfs Driver Unloaded\n");
}

module_init(sysfs_driver_init);
module_exit(sysfs_driver_exit);
