#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/i2c.h>
#include <linux/of.h>

#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>

#include <linux/uaccess.h>


#define DEVICE_NAME     "bbb_i2c"
#define CLASS_NAME      "bbb_i2c_class"

#define I2C_MAX_BUF     256


/* =========================================================
 * GLOBAL VARIABLES
 * ========================================================= */

static dev_t dev_num;

static struct cdev i2c_cdev;

static struct class *i2c_class;

static struct device *i2c_device;

static struct i2c_client *i2c_client;


/* =========================================================
 * OPEN
 * ========================================================= */

static int bbb_i2c_open(struct inode *inode,
                        struct file *file)
{
    pr_info("BBB I2C: device opened\n");

    return 0;
}


/* =========================================================
 * RELEASE
 * ========================================================= */

static int bbb_i2c_release(struct inode *inode,
                           struct file *file)
{
    pr_info("BBB I2C: device closed\n");

    return 0;
}


/* =========================================================
 * WRITE
 *
 * User:
 *
 *     echo "HELLO" > /dev/bbb_i2c
 *
 * ========================================================= */

static ssize_t bbb_i2c_write(struct file *file,
                             const char __user *buffer,
                             size_t count,
                             loff_t *offset)
{
    char tx_buf[I2C_MAX_BUF];

    int ret;


    if (count == 0)
        return 0;


    if (count > I2C_MAX_BUF)
        return -EINVAL;


    /* -----------------------------------------------------
     * Copy data from user space to kernel space
     * ----------------------------------------------------- */

    if (copy_from_user(tx_buf,
                       buffer,
                       count))
    {
        return -EFAULT;
    }


    /* -----------------------------------------------------
     * Send data to I2C slave
     * ----------------------------------------------------- */

    ret = i2c_master_send(i2c_client,
                          tx_buf,
                          count);


    if (ret < 0)
    {
        pr_err("BBB I2C: i2c_master_send failed: %d\n",
               ret);

        return ret;
    }


    pr_info("BBB I2C: transmitted %d bytes\n",
            ret);


    return ret;
}


/* =========================================================
 * READ
 *
 * User:
 *
 *     read() from /dev/bbb_i2c
 *
 * ========================================================= */

static ssize_t bbb_i2c_read(struct file *file,
                            char __user *buffer,
                            size_t count,
                            loff_t *offset)
{
    char rx_buf[I2C_MAX_BUF];

    int ret;


    if (count == 0)
        return 0;


    if (count > I2C_MAX_BUF)
        return -EINVAL;


    /* -----------------------------------------------------
     * Receive data from I2C slave
     * ----------------------------------------------------- */

    ret = i2c_master_recv(i2c_client,
                          rx_buf,
                          count);


    if (ret < 0)
    {
        pr_err("BBB I2C: i2c_master_recv failed: %d\n",
               ret);

        return ret;
    }


    /* -----------------------------------------------------
     * Copy received data to user space
     * ----------------------------------------------------- */

    if (copy_to_user(buffer,
                     rx_buf,
                     ret))
    {
        return -EFAULT;
    }


    pr_info("BBB I2C: received %d bytes\n",
            ret);


    return ret;
}


/* =========================================================
 * FILE OPERATIONS
 * ========================================================= */

static const struct file_operations i2c_fops =
{
    .owner = THIS_MODULE,

    .open = bbb_i2c_open,

    .release = bbb_i2c_release,

    .read = bbb_i2c_read,

    .write = bbb_i2c_write,
};


/* =========================================================
 * I2C PROBE
 * ========================================================= */

static int bbb_i2c_probe(struct i2c_client *client,
                         const struct i2c_device_id *id)
{
    int ret;


    pr_info("BBB I2C: probe called\n");


    /* -----------------------------------------------------
     * Save I2C client pointer
     * ----------------------------------------------------- */

    i2c_client = client;


    pr_info("BBB I2C: slave address = 0x%02x\n",
            client->addr);


    pr_info("BBB I2C: adapter = %s\n",
            client->adapter->name);


    /* -----------------------------------------------------
     * Check I2C adapter functionality
     * ----------------------------------------------------- */

    if (!i2c_check_functionality(client->adapter,
                                 I2C_FUNC_I2C))
    {
        pr_err("BBB I2C: adapter does not support I2C\n");

        i2c_client = NULL;

        return -EOPNOTSUPP;
    }


    pr_info("BBB I2C: adapter supports I2C\n");


    /* =====================================================
     * CHARACTER DEVICE
     * ===================================================== */


    /* -----------------------------------------------------
     * Allocate major/minor number
     * ----------------------------------------------------- */

    ret = alloc_chrdev_region(&dev_num,
                              0,
                              1,
                              DEVICE_NAME);


    if (ret < 0)
    {
        pr_err("BBB I2C: alloc_chrdev_region failed\n");

        i2c_client = NULL;

        return ret;
    }


    pr_info("BBB I2C: major = %d\n",
            MAJOR(dev_num));

    pr_info("BBB I2C: minor = %d\n",
            MINOR(dev_num));


    /* -----------------------------------------------------
     * Initialize cdev
     * ----------------------------------------------------- */

    cdev_init(&i2c_cdev,
              &i2c_fops);

    i2c_cdev.owner = THIS_MODULE;


    /* -----------------------------------------------------
     * Add cdev
     * ----------------------------------------------------- */

    ret = cdev_add(&i2c_cdev,
                   dev_num,
                   1);


    if (ret < 0)
    {
        pr_err("BBB I2C: cdev_add failed\n");

        goto unregister_chrdev;
    }


    /* -----------------------------------------------------
     * Create class
     *
     * Kernel 5.10 API
     * ----------------------------------------------------- */

    i2c_class = class_create(THIS_MODULE,
                             CLASS_NAME);


    if (IS_ERR(i2c_class))
    {
        ret = PTR_ERR(i2c_class);

        pr_err("BBB I2C: class_create failed\n");

        goto delete_cdev;
    }


    /* -----------------------------------------------------
     * Create device
     *
     * Creates:
     *
     * /dev/bbb_i2c
     * ----------------------------------------------------- */

    i2c_device = device_create(i2c_class,
                               NULL,
                               dev_num,
                               NULL,
                               DEVICE_NAME);


    if (IS_ERR(i2c_device))
    {
        ret = PTR_ERR(i2c_device);

        pr_err("BBB I2C: device_create failed\n");

        goto destroy_class;
    }


    pr_info("BBB I2C: /dev/%s created\n",
            DEVICE_NAME);


    return 0;


/* =========================================================
 * ERROR HANDLING
 * ========================================================= */

destroy_class:

    class_destroy(i2c_class);


delete_cdev:

    cdev_del(&i2c_cdev);


unregister_chrdev:

    unregister_chrdev_region(dev_num,
                             1);

    i2c_client = NULL;

    return ret;
}


/* =========================================================
 * I2C REMOVE
 * ========================================================= */

static int bbb_i2c_remove(struct i2c_client *client)
{
    pr_info("BBB I2C: remove called\n");


    /* -----------------------------------------------------
     * Remove /dev/bbb_i2c
     * ----------------------------------------------------- */

    device_destroy(i2c_class,
                   dev_num);


    /* -----------------------------------------------------
     * Destroy class
     * ----------------------------------------------------- */

    class_destroy(i2c_class);


    /* -----------------------------------------------------
     * Remove cdev
     * ----------------------------------------------------- */

    cdev_del(&i2c_cdev);


    /* -----------------------------------------------------
     * Release major/minor number
     * ----------------------------------------------------- */

    unregister_chrdev_region(dev_num,
                             1);


    /* -----------------------------------------------------
     * Clear client pointer
     * ----------------------------------------------------- */

    i2c_client = NULL;


    pr_info("BBB I2C: driver removed\n");


    return 0;
}


/* =========================================================
 * DEVICE TREE MATCH TABLE
 * ========================================================= */

static const struct of_device_id bbb_i2c_of_match[] =
{
    {
        .compatible = "madhu,bbb-i2c"
    },

    { }
};


MODULE_DEVICE_TABLE(of,
                    bbb_i2c_of_match);


/* =========================================================
 * I2C DEVICE ID TABLE
 * ========================================================= */

static const struct i2c_device_id bbb_i2c_id[] =
{
    {
        "bbb_i2c",
        0
    },

    { }
};


MODULE_DEVICE_TABLE(i2c,
                    bbb_i2c_id);


/* =========================================================
 * I2C DRIVER
 * ========================================================= */

static struct i2c_driver bbb_i2c_driver =
{
    .driver =
    {
        .name = "bbb_i2c",

        .of_match_table = bbb_i2c_of_match,
    },

    .probe = bbb_i2c_probe,

    .remove = bbb_i2c_remove,

    .id_table = bbb_i2c_id,
};


/* =========================================================
 * MODULE INIT
 * ========================================================= */

static int __init bbb_i2c_init(void)
{
    pr_info("BBB I2C: module init\n");

    return i2c_add_driver(&bbb_i2c_driver);
}


/* =========================================================
 * MODULE EXIT
 * ========================================================= */

static void __exit bbb_i2c_exit(void)
{
    pr_info("BBB I2C: module exit\n");

    i2c_del_driver(&bbb_i2c_driver);
}


/* =========================================================
 * MODULE INFORMATION
 * ========================================================= */

module_init(bbb_i2c_init);

module_exit(bbb_i2c_exit);


MODULE_LICENSE("GPL");

MODULE_AUTHOR("Madhu");

MODULE_DESCRIPTION("BeagleBone Black I2C Client Driver");

MODULE_VERSION("1.0");
