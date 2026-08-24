#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Madhu");
MODULE_DESCRIPTION("Assignment 13 - Dummy Interrupt Handler");
MODULE_VERSION("1.0");

/* Change this IRQ if needed */
#define IRQ_NO 1

static int irq_count;
static int dev_id;

/*------------------------------------------------------*/
/* Interrupt Handler */
/*------------------------------------------------------*/
static irqreturn_t my_irq_handler(int irq, void *dev)
{
    irq_count++;

    printk(KERN_INFO "Interrupt Occurred : %d\n", irq_count);

    return IRQ_HANDLED;
}

/*------------------------------------------------------*/
/* Init */
/*------------------------------------------------------*/
static int __init my_irq_init(void)
{
    int ret;

    irq_count = 0;

    ret = request_irq(IRQ_NO,
                      my_irq_handler,
                      IRQF_SHARED,
                      "my_irq_driver",
                      &dev_id);

    if (ret) {
        printk(KERN_ERR "Failed to register IRQ %d\n", IRQ_NO);
        return ret;
    }

    printk(KERN_INFO "IRQ Driver Loaded\n");
    printk(KERN_INFO "Registered IRQ = %d\n", IRQ_NO);

    return 0;
}

/*------------------------------------------------------*/
/* Exit */
/*------------------------------------------------------*/
static void __exit my_irq_exit(void)
{
    free_irq(IRQ_NO, &dev_id);

    printk(KERN_INFO "Total Interrupt Count = %d\n",
           irq_count);

    printk(KERN_INFO "IRQ Driver Unloaded\n");
}

module_init(my_irq_init);
module_exit(my_irq_exit);
