#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/interrupt.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Madhu");
MODULE_DESCRIPTION("Top Half and Bottom Half using Timer and Tasklet");

static struct timer_list my_timer;
static struct tasklet_struct my_tasklet;

/* Bottom Half */
static void tasklet_function(struct tasklet_struct *t)
{
    printk(KERN_INFO "Bottom Half Executing\n");
}

/* Top Half (Interrupt Handler Simulation) */
static void timer_callback(struct timer_list *t)
{
    printk(KERN_INFO "Timer Expired\n");

    printk(KERN_INFO "Top Half Executing\n");

    tasklet_schedule(&my_tasklet);

    /* Restart timer after 1 second */
    mod_timer(&my_timer, jiffies + msecs_to_jiffies(1000));
}

static int __init top_bottom_init(void)
{
    printk(KERN_INFO "Top Half Bottom Half Module Loaded\n");

    /* Initialize Tasklet */
    tasklet_setup(&my_tasklet, tasklet_function);

    /* Initialize Timer */
    timer_setup(&my_timer, timer_callback, 0);

    /* Start Timer */
    mod_timer(&my_timer, jiffies + msecs_to_jiffies(1000));

    return 0;
}

static void __exit top_bottom_exit(void)
{
    timer_delete_sync(&my_timer);

    tasklet_kill(&my_tasklet);

    printk(KERN_INFO "Module Removed\n");
}

module_init(top_bottom_init);
module_exit(top_bottom_exit);
