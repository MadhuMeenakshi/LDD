#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/spi/spi.h>
#include <linux/of.h>

#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>

#include <linux/uaccess.h>


#define DEVICE_NAME     "bbb_spi"
#define CLASS_NAME      "bbb_spi_class"

#define SPI_MAX_BUF     256


/* =========================================================
 * GLOBAL VARIABLES
 * ========================================================= */

static dev_t dev_num;

static struct cdev spi_cdev;

static struct class *spi_class;

static struct device *spi_device;

static struct spi_device *spi_dev;


/* =========================================================
 * OPEN
 * ========================================================= */

static int bbb_spi_open(struct inode *inode,
                        struct file *file)
{
    pr_info("BBB SPI: device opened\n");

    return 0;
}


/* =========================================================
 * RELEASE
 * ========================================================= */

static int bbb_spi_release(struct inode *inode,
                           struct file *file)
{
    pr_info("BBB SPI: device closed\n");

    return 0;
}


/* =========================================================
 * WRITE
 *
 * User space:
 *
 *     echo "HELLO" > /dev/bbb_spi
 *
 * Data is transmitted through SPI.
 * ========================================================= */

static ssize_t bbb_spi_write(struct file *file,
                             const char __user *buffer,
                             size_t count,
                             loff_t *offset)
{
    char tx_buf[SPI_MAX_BUF];

    struct spi_transfer transfer = {
        .tx_buf = tx_buf,
        .len = count,
    };

    struct spi_message message;

    int ret;


    /* Check buffer size */

    if (count == 0)
        return 0;

    if (count > SPI_MAX_BUF)
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
     * Initialize SPI message
     * ----------------------------------------------------- */

    spi_message_init(&message);


    /* -----------------------------------------------------
     * Add transfer to message
     * ----------------------------------------------------- */

    spi_message_add_tail(&transfer,
                         &message);


    /* -----------------------------------------------------
     * Perform SPI transfer
     * ----------------------------------------------------- */

    ret = spi_sync(spi_dev,
                   &message);


    if (ret < 0)
    {
        pr_err("BBB SPI: spi_sync failed: %d\n",
               ret);

        return ret;
    }


    pr_info("BBB SPI: transmitted %zu bytes\n",
            count);


    return count;
}


/* =========================================================
 * READ
 *
 * User space:
 *
 *     read() from /dev/bbb_spi
 *
 * SPI is full duplex, therefore we provide a TX buffer
 * and receive the returned data in RX buffer.
 * ========================================================= */

static ssize_t bbb_spi_read(struct file *file,
                            char __user *buffer,
                            size_t count,
                            loff_t *offset)
{
    char tx_buf[SPI_MAX_BUF] = {0};

    char rx_buf[SPI_MAX_BUF] = {0};

    struct spi_transfer transfer = {
        .tx_buf = tx_buf,
        .rx_buf = rx_buf,
        .len = count,
    };

    struct spi_message message;

    int ret;


    /* Check buffer size */

    if (count == 0)
        return 0;

    if (count > SPI_MAX_BUF)
        return -EINVAL;


    /* -----------------------------------------------------
     * Initialize SPI message
     * ----------------------------------------------------- */

    spi_message_init(&message);


    /* -----------------------------------------------------
     * Add transfer
     * ----------------------------------------------------- */

    spi_message_add_tail(&transfer,
                         &message);


    /* -----------------------------------------------------
     * Perform SPI transfer
     * ----------------------------------------------------- */

    ret = spi_sync(spi_dev,
                   &message);


    if (ret < 0)
    {
        pr_err("BBB SPI: SPI read failed: %d\n",
               ret);

        return ret;
    }


    /* -----------------------------------------------------
     * Copy received data to user space
     * ----------------------------------------------------- */

    if (copy_to_user(buffer,
                     rx_buf,
                     count))
    {
        return -EFAULT;
    }


    pr_info("BBB SPI: received %zu bytes\n",
            count);


    return count;
}


/* =========================================================
 * FILE OPERATIONS
 * ========================================================= */

static const struct file_operations spi_fops =
{
    .owner = THIS_MODULE,

    .open = bbb_spi_open,

    .release = bbb_spi_release,

    .read = bbb_spi_read,

    .write = bbb_spi_write,
};


/* =========================================================
 * SPI PROBE
 * ========================================================= */

static int bbb_spi_probe(struct spi_device *spi)
{
    int ret;


    pr_info("BBB SPI: probe called\n");


    /* -----------------------------------------------------
     * Save SPI device pointer
     * ----------------------------------------------------- */

    spi_dev = spi;


    /* -----------------------------------------------------
     * Configure SPI mode
     *
     * SPI_MODE_0:
     *
     * CPOL = 0
     * CPHA = 0
     * ----------------------------------------------------- */

    spi->mode = SPI_MODE_0;


    /* -----------------------------------------------------
     * 8 bits per word
     * ----------------------------------------------------- */

    spi->bits_per_word = 8;


    /* -----------------------------------------------------
     * SPI clock = 1 MHz
     * ----------------------------------------------------- */

    spi->max_speed_hz = 1000000;


    /* -----------------------------------------------------
     * Apply SPI configuration
     * ----------------------------------------------------- */

    ret = spi_setup(spi);


    if (ret < 0)
    {
        pr_err("BBB SPI: spi_setup failed: %d\n",
               ret);

        spi_dev = NULL;

        return ret;
    }


    pr_info("BBB SPI: SPI configured successfully\n");

    pr_info("BBB SPI: mode = %d\n",
            spi->mode);

    pr_info("BBB SPI: bits per word = %d\n",
            spi->bits_per_word);

    pr_info("BBB SPI: speed = %u Hz\n",
            spi->max_speed_hz);


    /* =====================================================
     * CHARACTER DEVICE
     * ===================================================== */


    /* -----------------------------------------------------
     * Allocate major and minor number
     * ----------------------------------------------------- */

    ret = alloc_chrdev_region(&dev_num,
                              0,
                              1,
                              DEVICE_NAME);


    if (ret < 0)
    {
        pr_err("BBB SPI: alloc_chrdev_region failed\n");

        spi_dev = NULL;

        return ret;
    }


    pr_info("BBB SPI: major = %d\n",
            MAJOR(dev_num));

    pr_info("BBB SPI: minor = %d\n",
            MINOR(dev_num));


    /* -----------------------------------------------------
     * Initialize cdev
     * ----------------------------------------------------- */

    cdev_init(&spi_cdev,
              &spi_fops);

    spi_cdev.owner = THIS_MODULE;


    /* -----------------------------------------------------
     * Add cdev
     * ----------------------------------------------------- */

    ret = cdev_add(&spi_cdev,
                   dev_num,
                   1);


    if (ret < 0)
    {
        pr_err("BBB SPI: cdev_add failed\n");

        goto unregister_chrdev;
    }


    /* -----------------------------------------------------
     * Create class
     *
     * Kernel 5.10 API
     * ----------------------------------------------------- */

    spi_class = class_create(THIS_MODULE,
                             CLASS_NAME);


    if (IS_ERR(spi_class))
    {
        ret = PTR_ERR(spi_class);

        pr_err("BBB SPI: class_create failed\n");

        goto delete_cdev;
    }


    /* -----------------------------------------------------
     * Create device
     *
     * Creates:
     *
     * /dev/bbb_spi
     * ----------------------------------------------------- */

    spi_device = device_create(spi_class,
                               NULL,
                               dev_num,
                               NULL,
                               DEVICE_NAME);


    if (IS_ERR(spi_device))
    {
        ret = PTR_ERR(spi_device);

        pr_err("BBB SPI: device_create failed\n");

        goto destroy_class;
    }


    pr_info("BBB SPI: /dev/%s created\n",
            DEVICE_NAME);


    return 0;


/* =========================================================
 * ERROR HANDLING
 * ========================================================= */

destroy_class:

    class_destroy(spi_class);


delete_cdev:

    cdev_del(&spi_cdev);


unregister_chrdev:

    unregister_chrdev_region(dev_num,
                             1);

    spi_dev = NULL;

    return ret;
}


/* =========================================================
 * SPI REMOVE
 * ========================================================= */

static int bbb_spi_remove(struct spi_device *spi)
{
    pr_info("BBB SPI: remove called\n");


    /* -----------------------------------------------------
     * Remove /dev/bbb_spi
     * ----------------------------------------------------- */

    device_destroy(spi_class,
                   dev_num);


    /* -----------------------------------------------------
     * Destroy class
     * ----------------------------------------------------- */

    class_destroy(spi_class);


    /* -----------------------------------------------------
     * Remove cdev
     * ----------------------------------------------------- */

    cdev_del(&spi_cdev);


    /* -----------------------------------------------------
     * Release device number
     * ----------------------------------------------------- */

    unregister_chrdev_region(dev_num,
                             1);


    /* -----------------------------------------------------
     * Clear global SPI pointer
     * ----------------------------------------------------- */

    spi_dev = NULL;


    pr_info("BBB SPI: driver removed\n");


    return 0;
}


/* =========================================================
 * DEVICE TREE MATCH TABLE
 * ========================================================= */

static const struct of_device_id bbb_spi_of_match[] =
{
    {
        .compatible = "madhu,bbb-spi"
    },

    { }
};


MODULE_DEVICE_TABLE(of,
                    bbb_spi_of_match);


/* =========================================================
 * SPI DRIVER STRUCTURE
 * ========================================================= */

static struct spi_driver bbb_spi_driver =
{
    .driver =
    {
        .name = "bbb_spi",

        .of_match_table = bbb_spi_of_match,
    },

    .probe = bbb_spi_probe,

    .remove = bbb_spi_remove,
};


/* =========================================================
 * MODULE INIT
 * ========================================================= */

static int __init bbb_spi_init(void)
{
    pr_info("BBB SPI: module init\n");


    return spi_register_driver(&bbb_spi_driver);
}


/* =========================================================
 * MODULE EXIT
 * ========================================================= */

static void __exit bbb_spi_exit(void)
{
    pr_info("BBB SPI: module exit\n");


    spi_unregister_driver(&bbb_spi_driver);
}


/* =========================================================
 * MODULE INFORMATION
 * ========================================================= */

module_init(bbb_spi_init);

module_exit(bbb_spi_exit);


MODULE_LICENSE("GPL");

MODULE_AUTHOR("Madhu");

MODULE_DESCRIPTION("BeagleBone Black SPI Client Driver");

MODULE_VERSION("1.0");
