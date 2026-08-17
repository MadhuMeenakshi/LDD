#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/string.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Madhu");
MODULE_DESCRIPTION("Dynamic sysfs Attribute");

static struct kobject *my_kobj;

static char mode[20] = "AUTO";

/* Read */

static ssize_t mode_show(struct kobject *kobj,
                         struct kobj_attribute *attr,
                         char *buf)
{
    return sprintf(buf, "%s\n", mode);
}

/* Write */

static ssize_t mode_store(struct kobject *kobj,
                          struct kobj_attribute *attr,
                          const char *buf,
                          size_t count)
{
    char temp[20];

    if (count >= sizeof(temp))
        return -EINVAL;

    strncpy(temp, buf, count);
    temp[count] = '\0';

    strim(temp);

    if (!strcmp(temp, "AUTO") ||
        !strcmp(temp, "MANUAL") ||
        !strcmp(temp, "OFF"))
    {
        strcpy(mode, temp);

        printk(KERN_INFO "Mode Changed : %s\n", mode);
    }
    else
    {
        printk(KERN_ERR "Invalid Mode\n");
        return -EINVAL;
    }

    return count;
}

static struct kobj_attribute mode_attribute =
        __ATTR(mode, 0664,
               mode_show,
               mode_store);

/* Init */

static int __init mode_init(void)
{
    int ret;

    my_kobj = kobject_create_and_add("mydriver",
                                     kernel_kobj);

    if (!my_kobj)
        return -ENOMEM;

    ret = sysfs_create_file(my_kobj,
                            &mode_attribute.attr);

    if (ret)
    {
        kobject_put(my_kobj);
        return ret;
    }

    printk(KERN_INFO "Mode sysfs Created\n");

    return 0;
}

/* Exit */

static void __exit mode_exit(void)
{
    sysfs_remove_file(my_kobj,
                      &mode_attribute.attr);

    kobject_put(my_kobj);

    printk(KERN_INFO "Mode sysfs Removed\n");
}

module_init(mode_init);
module_exit(mode_exit);
