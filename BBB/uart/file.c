#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/io.h>

#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>

#include <linux/uaccess.h>


#define DEVICE_NAME "bbb_uart"
#define CLASS_NAME  "bbb_uart_class"


/* ---------------------------------------------------------
 * Global variables
 * --------------------------------------------------------- */

static dev_t dev_num;

static struct cdev uart_cdev;

static struct class *uart_class;

static struct device *uart_device;

static void __iomem *uart_base;


/* ---------------------------------------------------------
 * UART register offsets
 *
 * AM335x UART registers
 * --------------------------------------------------------- */

#define UART_RHR       0x00
#define UART_THR       0x00
#define UART_DLL       0x00
#define UART_DLH       0x04

#define UART_IER       0x04
#define UART_IIR       0x08
#define UART_FCR       0x08
#define UART_LCR       0x0C
#define UART_MCR       0x10
#define UART_LSR       0x14
#define UART_MDR1      0x20


/* LSR bits */

#define UART_LSR_DR    BIT(0)
#define UART_LSR_THRE  BIT(5)


/* LCR */

#define UART_LCR_8N1   0x03


/* ---------------------------------------------------------
 * OPEN
 * --------------------------------------------------------- */

static int uart_open(struct inode *inode,
                     struct file *file)
{
    pr_info("BBB UART: device opened\n");

    return 0;
}


/* ---------------------------------------------------------
 * RELEASE
 * --------------------------------------------------------- */

static int uart_release(struct inode *inode,
                        struct file *file)
{
    pr_info("BBB UART: device closed\n");

    return 0;
}


/* ---------------------------------------------------------
 * WRITE
 *
 * User:
 *
 * echo "Hello" > /dev/bbb_uart
 *
 * --------------------------------------------------------- */

static ssize_t uart_write(struct file *file,
                          const char __user *buffer,
                          size_t count,
                          loff_t *offset)
{
    char data[256];
    size_t i;


    if (count >= sizeof(data))
        return -EINVAL;


    if (copy_from_user(data, buffer, count))
        return -EFAULT;


    /*
     * Wait until UART transmitter is ready
     * and send each character.
     */

    for (i = 0; i < count; i++)
    {
        /*
         * Wait until THR is empty.
         */

        while (!(readb(uart_base + UART_LSR) &
                 UART_LSR_THRE))
        {
            cpu_relax();
        }


        /*
         * Write character to transmit holding register.
         */

        writeb(data[i],
               uart_base + UART_THR);
    }


    pr_info("BBB UART: transmitted %zu bytes\n",
            count);


    return count;
}


/* ---------------------------------------------------------
 * READ
 *
 * Reads one character from UART.
 *
 * --------------------------------------------------------- */

static ssize_t uart_read(struct file *file,
                         char __user *buffer,
                         size_t count,
                         loff_t *offset)
{
    char data;
    u8 lsr;


    if (count < 1)
        return -EINVAL;


    /*
     * Check whether data is available.
     */

    lsr = readb(uart_base + UART_LSR);


    if (!(lsr & UART_LSR_DR))
        return 0;


    /*
     * Read received character.
     */

    data = readb(uart_base + UART_RHR);


    /*
     * Copy character to user space.
     */

    if (copy_to_user(buffer,
                     &data,
                     1))
    {
        return -EFAULT;
    }


    pr_info("BBB UART: received '%c'\n",
            data);


    return 1;
}


/* ---------------------------------------------------------
 * FILE OPERATIONS
 * --------------------------------------------------------- */

static const struct file_operations uart_fops =
{
    .owner   = THIS_MODULE,

    .open    = uart_open,

    .release = uart_release,

    .read    = uart_read,

    .write   = uart_write,
};


/* ---------------------------------------------------------
 * UART PROBE
 * --------------------------------------------------------- */

static int uart_probe(struct platform_device *pdev)
{
    struct resource *res;

    int ret;


    pr_info("BBB UART: probe called\n");


    /* -----------------------------------------------------
     * 1. Get UART memory resource
     * ----------------------------------------------------- */

    res = platform_get_resource(pdev,
                                IORESOURCE_MEM,
                                0);


    if (!res)
    {
        pr_err("BBB UART: memory resource not found\n");

        return -ENODEV;
    }


    pr_info("BBB UART: resource start = 0x%llx\n",
            (unsigned long long)res->start);


    /* -----------------------------------------------------
     * 2. Map UART registers into kernel virtual address
     * ----------------------------------------------------- */

    uart_base = devm_ioremap_resource(&pdev->dev,
                                      res);


    if (IS_ERR(uart_base))
    {
        ret = PTR_ERR(uart_base);

        pr_err("BBB UART: ioremap failed\n");

        return ret;
    }


    pr_info("BBB UART: registers mapped\n");


    /* -----------------------------------------------------
     * 3. Allocate character device number
     * ----------------------------------------------------- */

    ret = alloc_chrdev_region(&dev_num,
                              0,
                              1,
                              DEVICE_NAME);


    if (ret < 0)
    {
        pr_err("BBB UART: alloc_chrdev_region failed\n");

        return ret;
    }


    /* -----------------------------------------------------
     * 4. Initialize cdev
     * ----------------------------------------------------- */

    cdev_init(&uart_cdev,
              &uart_fops);

    uart_cdev.owner = THIS_MODULE;


    /* -----------------------------------------------------
     * 5. Add cdev
     * ----------------------------------------------------- */

    ret = cdev_add(&uart_cdev,
                   dev_num,
                   1);


    if (ret < 0)
    {
        pr_err("BBB UART: cdev_add failed\n");

        goto unregister_chrdev;
    }


    /* -----------------------------------------------------
     * 6. Create class
     *
     * Kernel 5.10 API
     * ----------------------------------------------------- */

    uart_class = class_create(THIS_MODULE,
                              CLASS_NAME);


    if (IS_ERR(uart_class))
    {
        ret = PTR_ERR(uart_class);

        pr_err("BBB UART: class_create failed\n");

        goto delete_cdev;
    }


    /* -----------------------------------------------------
     * 7. Create /dev/bbb_uart
     * ----------------------------------------------------- */

    uart_device = device_create(uart_class,
                                NULL,
                                dev_num,
                                NULL,
                                DEVICE_NAME);


    if (IS_ERR(uart_device))
    {
        ret = PTR_ERR(uart_device);

        pr_err("BBB UART: device_create failed\n");

        goto destroy_class;
    }


    pr_info("BBB UART: /dev/%s created\n",
            DEVICE_NAME);


    return 0;


/* ---------------------------------------------------------
 * Error handling
 * --------------------------------------------------------- */

destroy_class:

    class_destroy(uart_class);


delete_cdev:

    cdev_del(&uart_cdev);


unregister_chrdev:

    unregister_chrdev_region(dev_num,
                             1);

    return ret;
}


/* ---------------------------------------------------------
 * UART REMOVE
 * --------------------------------------------------------- */

static int uart_remove(struct platform_device *pdev)
{
    pr_info("BBB UART: remove called\n");


    device_destroy(uart_class,
                   dev_num);


    class_destroy(uart_class);


    cdev_del(&uart_cdev);


    unregister_chrdev_region(dev_num,
                             1);


    pr_info("BBB UART: driver removed\n");


    return 0;
}


/* ---------------------------------------------------------
 * DEVICE TREE MATCH
 * --------------------------------------------------------- */

static const struct of_device_id uart_of_match[] =
{
    {
        .compatible = "madhu,bbb-uart"
    },

    { }
};


MODULE_DEVICE_TABLE(of,
                    uart_of_match);


/* ---------------------------------------------------------
 * PLATFORM DRIVER
 * --------------------------------------------------------- */

static struct platform_driver uart_driver =
{
    .probe  = uart_probe,

    .remove = uart_remove,

    .driver =
    {
        .name = "bbb_uart_driver",

        .of_match_table = uart_of_match,
    },
};


/* ---------------------------------------------------------
 * MODULE INIT
 * --------------------------------------------------------- */

static int __init uart_init(void)
{
    pr_info("BBB UART: module init\n");

    return platform_driver_register(&uart_driver);
}


/* ---------------------------------------------------------
 * MODULE EXIT
 * --------------------------------------------------------- */

static void __exit uart_exit(void)
{
    pr_info("BBB UART: module exit\n");

    platform_driver_unregister(&uart_driver);
}


/* ---------------------------------------------------------
 * MODULE INFORMATION
 * --------------------------------------------------------- */

module_init(uart_init);

module_exit(uart_exit);


MODULE_LICENSE("GPL");

MODULE_AUTHOR("Madhu");

MODULE_DESCRIPTION("BeagleBone Black UART Platform Character Driver");

MODULE_VERSION("1.0");
