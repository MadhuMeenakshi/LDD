#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/timekeeping.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Madhu");
MODULE_DESCRIPTION("Assignment 10 - Kernel Thread");
MODULE_VERSION("1.0");

static struct task_struct *thread;

/* Kernel Thread Function */
static int thread_function(void *data)
{
    struct timespec64 ts;

    while (!kthread_should_stop()) {

        ktime_get_real_ts64(&ts);

        printk(KERN_INFO "Current Time : %lld.%09ld\n",
               (long long)ts.tv_sec,
               ts.tv_nsec);

        ssleep(5);
    }

    printk(KERN_INFO "Kernel Thread Stopped\n");

    return 0;
}

/* Module Init */
static int __init thread_init(void)
{
    thread = kthread_run(thread_function,
                         NULL,
                         "my_kernel_thread");

    if (IS_ERR(thread)) {
        printk(KERN_ERR "Failed to create thread\n");
        return PTR_ERR(thread);
    }

    printk(KERN_INFO "Kernel Thread Started\n");

    return 0;
}

/* Module Exit */
static void __exit thread_exit(void)
{
    if (thread)
        kthread_stop(thread);

    printk(KERN_INFO "Module Removed\n");
}

module_init(thread_init);
module_exit(thread_exit);
