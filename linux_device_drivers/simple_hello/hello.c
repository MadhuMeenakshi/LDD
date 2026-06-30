
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int __init init_fn(void)
{
	pr_info("just an init module\n");
	return 0;
}

static void __exit exit_fn(void)
{
	pr_info("just an exit function\n");
}



module_init(init_fn);
module_exit(exit_fn);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("MADHU");
MODULE_DESCRIPTION("just a init/exit module");
MODULE_VERSION("1.0");
