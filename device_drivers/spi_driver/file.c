#include <linux/module.h>
#include <linux/init.h>
#include <linux/spi/spi.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Madhu");
MODULE_DESCRIPTION("Assignment 16 - SPI Driver");
MODULE_VERSION("1.0");

static int my_spi_probe(struct spi_device *spi)
{
    u8 tx_buf[4] = {0x11, 0x22, 0x33, 0x44};
    u8 rx_buf[4] = {0};

    struct spi_transfer transfer = {
        .tx_buf = tx_buf,
        .rx_buf = rx_buf,
        .len = sizeof(tx_buf),
    };

    struct spi_message message;

    printk(KERN_INFO "SPI Driver Probe\n");
    printk(KERN_INFO "SPI Device: %s\n", dev_name(&spi->dev));

    spi_message_init(&message);
    spi_message_add_tail(&transfer, &message);

    spi_sync(spi, &message);

    printk(KERN_INFO "Dummy SPI Transfer Completed\n");

    return 0;
}

static void my_spi_remove(struct spi_device *spi)
{
    printk(KERN_INFO "SPI Driver Removed\n");
}

static const struct spi_device_id my_spi_id[] = {
    { "dummy_spi", 0 },
    { }
};
MODULE_DEVICE_TABLE(spi, my_spi_id);

static const struct of_device_id my_spi_of_match[] = {
    { .compatible = "madhu,dummy_spi" },
    { }
};
MODULE_DEVICE_TABLE(of, my_spi_of_match);

static struct spi_driver my_spi_driver = {
    .driver = {
        .name = "dummy_spi",
        .of_match_table = my_spi_of_match,
    },
    .probe = my_spi_probe,
    .remove = my_spi_remove,
    .id_table = my_spi_id,
};

static int __init my_spi_init(void)
{
    printk(KERN_INFO "SPI Driver Loaded\n");
    return spi_register_driver(&my_spi_driver);
}

static void __exit my_spi_exit(void)
{
    spi_unregister_driver(&my_spi_driver);
    printk(KERN_INFO "SPI Driver Unloaded\n");
}

module_init(my_spi_init);
module_exit(my_spi_exit);
