#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/jiffies.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Madhu");
MODULE_DESCRIPTION("Software Interrupt Counter using Kernel Timer");

static struct timer_list my_timer;
static int interrupt_count = 0;

/* Timer callback function */
static void timer_callback(struct timer_list *timer)
{
    interrupt_count++;

    printk(KERN_INFO "Interrupt No : %d\n", interrupt_count);

    /* Restart timer after 1 second */
    mod_timer(&my_timer, jiffies + msecs_to_jiffies(1000));
}

/* Module Init */
static int __init timer_init(void)
{
    printk(KERN_INFO "Timer Module Loaded\n");

    /* Initialize timer */
    timer_setup(&my_timer, timer_callback, 0);

    /* Start timer */
    mod_timer(&my_timer, jiffies + msecs_to_jiffies(1000));

    return 0;
}

/* Module Exit */
static void __exit timer_exit(void)
{
    /* Delete timer synchronously */
    timer_delete_sync(&my_timer);

    printk(KERN_INFO "Timer Module Removed\n");
}

module_init(timer_init);
module_exit(timer_exit);
