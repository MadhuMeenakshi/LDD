#include <linux/module.h>
#include <linux/init.h>
#include <linux/i2c.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Madhu");
MODULE_DESCRIPTION("Assignment 15 - Dummy I2C Driver");
MODULE_VERSION("1.0");

/* Dummy Registers */
static u8 dummy_regs[256];

/*-------------------------------------------------------*/
/* Probe */
/*-------------------------------------------------------*/
static int my_i2c_probe(struct i2c_client *client)
{
    printk(KERN_INFO "I2C Driver Probe\n");
    printk(KERN_INFO "Device Address : 0x%02X\n", client->addr);

    dummy_regs[0] = 0x55;
    dummy_regs[1] = 0xAA;

    printk(KERN_INFO "Register[0] = 0x%02X\n", dummy_regs[0]);
    printk(KERN_INFO "Register[1] = 0x%02X\n", dummy_regs[1]);

    return 0;
}

/*-------------------------------------------------------*/
/* Remove */
/*-------------------------------------------------------*/
static void my_i2c_remove(struct i2c_client *client)
{
    printk(KERN_INFO "I2C Driver Removed\n");
}

/*-------------------------------------------------------*/
/* Device Tree Match Table */
/*-------------------------------------------------------*/
static const struct of_device_id my_i2c_of_match[] = {
    { .compatible = "madhu,dummy_sensor" },
    { }
};
MODULE_DEVICE_TABLE(of, my_i2c_of_match);

/*-------------------------------------------------------*/
/* I2C Device ID Table */
/*-------------------------------------------------------*/
static const struct i2c_device_id my_i2c_id[] = {
    { "dummy_sensor", 0 },
    { }
};
MODULE_DEVICE_TABLE(i2c, my_i2c_id);

/*-------------------------------------------------------*/
/* I2C Driver */
/*-------------------------------------------------------*/
static struct i2c_driver my_i2c_driver = {
    .driver = {
        .name = "dummy_sensor",
        .of_match_table = my_i2c_of_match,
    },
    .probe = my_i2c_probe,
    .remove = my_i2c_remove,
    .id_table = my_i2c_id,
};

/*-------------------------------------------------------*/
/* Init */
/*-------------------------------------------------------*/
static int __init my_i2c_init(void)
{
    printk(KERN_INFO "Dummy I2C Driver Loaded\n");
    return i2c_add_driver(&my_i2c_driver);
}

/*-------------------------------------------------------*/
/* Exit */
/*-------------------------------------------------------*/
static void __exit my_i2c_exit(void)
{
    i2c_del_driver(&my_i2c_driver);
    printk(KERN_INFO "Dummy I2C Driver Unloaded\n");
}

module_init(my_i2c_init);
module_exit(my_i2c_exit);
