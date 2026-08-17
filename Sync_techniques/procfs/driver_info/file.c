#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Madhu");
MODULE_DESCRIPTION("Driver Information using sysfs");

static struct kobject *device_kobj;

/* Driver Information */

static int temperature = 35;
static int voltage = 33;      /* represents 3.3V */
static char status[] = "Running";

/* Temperature */

static ssize_t temperature_show(struct kobject *kobj,
                                struct kobj_attribute *attr,
                                char *buf)
{
    return sprintf(buf, "%d C\n", temperature);
}

static struct kobj_attribute temperature_attr =
        __ATTR(temperature, 0444, temperature_show, NULL);

/* Voltage */

static ssize_t voltage_show(struct kobject *kobj,
                            struct kobj_attribute *attr,
                            char *buf)
{
    return sprintf(buf, "%d.%d V\n",
                   voltage / 10,
                   voltage % 10);
}

static struct kobj_attribute voltage_attr =
        __ATTR(voltage, 0444, voltage_show, NULL);

/* Status */

static ssize_t status_show(struct kobject *kobj,
                           struct kobj_attribute *attr,
                           char *buf)
{
    return sprintf(buf, "%s\n", status);
}

static struct kobj_attribute status_attr =
        __ATTR(status, 0444, status_show, NULL);

/* Module Init */

static int __init device_info_init(void)
{
    int ret;

    device_kobj = kobject_create_and_add("device", kernel_kobj);

    if (!device_kobj)
        return -ENOMEM;

    ret = sysfs_create_file(device_kobj, &temperature_attr.attr);
    if (ret)
        goto error;

    ret = sysfs_create_file(device_kobj, &voltage_attr.attr);
    if (ret)
        goto error;

    ret = sysfs_create_file(device_kobj, &status_attr.attr);
    if (ret)
        goto error;

    printk(KERN_INFO "Driver Information sysfs Created\n");

    return 0;

error:
    kobject_put(device_kobj);
    return ret;
}

/* Module Exit */

static void __exit device_info_exit(void)
{
    sysfs_remove_file(device_kobj, &temperature_attr.attr);
    sysfs_remove_file(device_kobj, &voltage_attr.attr);
    sysfs_remove_file(device_kobj, &status_attr.attr);

    kobject_put(device_kobj);

    printk(KERN_INFO "Driver Information sysfs Removed\n");
}

module_init(device_info_init);
module_exit(device_info_exit);
