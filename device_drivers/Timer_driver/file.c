#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/jiffies.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Madhu");
MODULE_DESCRIPTION("Assignment 12 - Timer Driver");
MODULE_VERSION("1.0");

static struct timer_list my_timer;

/*-----------------------------------------------------------*/
/* Timer Callback */
/*-----------------------------------------------------------*/
static void timer_callback(struct timer_list *t)
{
    printk(KERN_INFO "Timer Expired - Every 3 Seconds\n");

    /* Restart timer */
    mod_timer(&my_timer, jiffies + msecs_to_jiffies(3000));
}

/*-----------------------------------------------------------*/
/* Module Init */
/*-----------------------------------------------------------*/
static int __init my_timer_init(void)
{
    timer_setup(&my_timer, timer_callback, 0);

    mod_timer(&my_timer, jiffies + msecs_to_jiffies(3000));

    printk(KERN_INFO "Timer Driver Loaded\n");

    return 0;
}

/*-----------------------------------------------------------*/
/* Module Exit */
/*-----------------------------------------------------------*/
static void __exit my_timer_exit(void)
{
    timer_delete_sync(&my_timer);

    printk(KERN_INFO "Timer Driver Unloaded\n");
}

module_init(my_timer_init);
module_exit(my_timer_exit);
