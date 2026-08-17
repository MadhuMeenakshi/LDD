#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("MADHU");
MODULE_DESCRIPTION("module with arguements");
MODULE_VERSION("1.0");

static int age = 0;
static char *name = "no name";

module_param(age,int,0644);
module_param(name,charp,0644);

MODULE_PARM_DESC(age,"age of the user");
MODULE_PARM_DESC(name,"name of the user");

static int __init ini_module(void)
{
	printk(KERN_INFO"printing the dynamic entities\n");
	printk(KERN_INFO"age is %d \n",age);
	printk(KERN_INFO"the name is %s \n",name);
	return 0;
}

static void __exit ex_module(void)
{
	printk(KERN_INFO"exiting the module\n");
}


module_init(ini_module);
module_exit(ex_module);
