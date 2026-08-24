#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/gpio/consumer.h>
#include <linux/of.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Madhu");
MODULE_DESCRIPTION("Assignment 17 - GPIO Driver");
MODULE_VERSION("1.0");

static struct gpio_desc *led_gpio;
static struct gpio_desc *button_gpio;

/*---------------------------------------------------------*/
/* Probe */
/*---------------------------------------------------------*/
static int my_gpio_probe(struct platform_device *pdev)
{
    int button;

    printk(KERN_INFO "GPIO Driver Probe\n");

    led_gpio = gpiod_get(&pdev->dev, "led", GPIOD_OUT_LOW);
    if (IS_ERR(led_gpio)) {
        printk(KERN_ERR "Failed to get LED GPIO\n");
        return PTR_ERR(led_gpio);
    }

    button_gpio = gpiod_get(&pdev->dev, "button", GPIOD_IN);
    if (IS_ERR(button_gpio)) {
        gpiod_put(led_gpio);
        printk(KERN_ERR "Failed to get Button GPIO\n");
        return PTR_ERR(button_gpio);
    }

    /* Turn LED ON */
    gpiod_set_value(led_gpio, 1);

    button = gpiod_get_value(button_gpio);

    printk(KERN_INFO "Button Status = %d\n", button);

    return 0;
}

/*---------------------------------------------------------*/
/* Remove */
/*---------------------------------------------------------*/
static void my_gpio_remove(struct platform_device *pdev)
{
    gpiod_set_value(led_gpio, 0);

    gpiod_put(led_gpio);
    gpiod_put(button_gpio);

    printk(KERN_INFO "GPIO Driver Removed\n");
}

/*---------------------------------------------------------*/
/* Device Tree Match */
/*---------------------------------------------------------*/
static const struct of_device_id gpio_of_match[] = {
    { .compatible = "madhu,gpio-demo" },
    { }
};

MODULE_DEVICE_TABLE(of, gpio_of_match);

/*---------------------------------------------------------*/
/* Platform Driver */
/*---------------------------------------------------------*/
static struct platform_driver gpio_driver = {
    .probe = my_gpio_probe,
    .remove = my_gpio_remove,
    .driver = {
        .name = "gpio_demo",
        .of_match_table = gpio_of_match,
    },
};

/*---------------------------------------------------------*/
/* Init */
/*---------------------------------------------------------*/
static int __init my_gpio_init(void)
{
    printk(KERN_INFO "GPIO Driver Loaded\n");
    return platform_driver_register(&gpio_driver);
}

/*---------------------------------------------------------*/
/* Exit */
/*---------------------------------------------------------*/
static void __exit my_gpio_exit(void)
{
    platform_driver_unregister(&gpio_driver);

    printk(KERN_INFO "GPIO Driver Unloaded\n");
}

module_init(my_gpio_init);
module_exit(my_gpio_exit);
