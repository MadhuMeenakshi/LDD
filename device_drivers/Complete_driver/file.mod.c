#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x092a35a2, "_copy_from_user" },
	{ 0x092a35a2, "_copy_to_user" },
	{ 0x7851be11, "__SCT__might_resched" },
	{ 0x40a621c5, "snprintf" },
	{ 0x7a5ffe84, "init_wait_entry" },
	{ 0xd272d446, "schedule" },
	{ 0x0db8d68d, "prepare_to_wait_event" },
	{ 0xc87f4bab, "finish_wait" },
	{ 0xbd03ed67, "random_kmalloc_seed" },
	{ 0x08bfc903, "kmalloc_caches" },
	{ 0xecd17989, "__kmalloc_cache_noprof" },
	{ 0xc1e6c71e, "__mutex_init" },
	{ 0x5403c125, "__init_waitqueue_head" },
	{ 0x9f222e1e, "alloc_chrdev_region" },
	{ 0xcb8b6ec6, "kfree" },
	{ 0x4c075f7d, "cdev_init" },
	{ 0x6459621a, "cdev_add" },
	{ 0xb6c08e4c, "class_create" },
	{ 0xf350d701, "device_create" },
	{ 0x92878b20, "proc_create" },
	{ 0x6464d17c, "kernel_kobj" },
	{ 0xed8368be, "kobject_create_and_add" },
	{ 0xbbf19d39, "sysfs_create_file_ns" },
	{ 0x3855aac9, "kthread_create_on_node" },
	{ 0xdba15b90, "wake_up_process" },
	{ 0x30a11079, "device_destroy" },
	{ 0xfd4b4a36, "class_destroy" },
	{ 0x0c92f06e, "cdev_del" },
	{ 0x0bc5fb0d, "unregister_chrdev_region" },
	{ 0x54216a7d, "proc_remove" },
	{ 0x2cc2c689, "sysfs_remove_file_ns" },
	{ 0x98b39dbb, "kobject_put" },
	{ 0x2e56bcda, "kthread_stop" },
	{ 0x5a844b26, "__x86_indirect_thunk_rax" },
	{ 0x11bacf83, "seq_read" },
	{ 0xd5bc7086, "seq_lseek" },
	{ 0x024d45a2, "single_release" },
	{ 0xd272d446, "__fentry__" },
	{ 0xe8213e80, "_printk" },
	{ 0xd272d446, "__x86_return_thunk" },
	{ 0xbd03ed67, "__ref_stack_chk_guard" },
	{ 0x67628f51, "msleep" },
	{ 0x5e505530, "kthread_should_stop" },
	{ 0x224a53e7, "get_random_bytes" },
	{ 0xf46d5bf3, "mutex_lock" },
	{ 0x16ab4215, "__wake_up" },
	{ 0xf46d5bf3, "mutex_unlock" },
	{ 0xd272d446, "__stack_chk_fail" },
	{ 0xd09b06f5, "kstrtoint" },
	{ 0xdd6830c7, "sprintf" },
	{ 0xce105414, "single_open" },
	{ 0x0d8b6c91, "seq_printf" },
	{ 0x546c19d9, "validate_usercopy_range" },
	{ 0xa61fd7aa, "__check_object_size" },
	{ 0x814e12e5, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0x092a35a2,
	0x092a35a2,
	0x7851be11,
	0x40a621c5,
	0x7a5ffe84,
	0xd272d446,
	0x0db8d68d,
	0xc87f4bab,
	0xbd03ed67,
	0x08bfc903,
	0xecd17989,
	0xc1e6c71e,
	0x5403c125,
	0x9f222e1e,
	0xcb8b6ec6,
	0x4c075f7d,
	0x6459621a,
	0xb6c08e4c,
	0xf350d701,
	0x92878b20,
	0x6464d17c,
	0xed8368be,
	0xbbf19d39,
	0x3855aac9,
	0xdba15b90,
	0x30a11079,
	0xfd4b4a36,
	0x0c92f06e,
	0x0bc5fb0d,
	0x54216a7d,
	0x2cc2c689,
	0x98b39dbb,
	0x2e56bcda,
	0x5a844b26,
	0x11bacf83,
	0xd5bc7086,
	0x024d45a2,
	0xd272d446,
	0xe8213e80,
	0xd272d446,
	0xbd03ed67,
	0x67628f51,
	0x5e505530,
	0x224a53e7,
	0xf46d5bf3,
	0x16ab4215,
	0xf46d5bf3,
	0xd272d446,
	0xd09b06f5,
	0xdd6830c7,
	0xce105414,
	0x0d8b6c91,
	0x546c19d9,
	0xa61fd7aa,
	0x814e12e5,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"_copy_from_user\0"
	"_copy_to_user\0"
	"__SCT__might_resched\0"
	"snprintf\0"
	"init_wait_entry\0"
	"schedule\0"
	"prepare_to_wait_event\0"
	"finish_wait\0"
	"random_kmalloc_seed\0"
	"kmalloc_caches\0"
	"__kmalloc_cache_noprof\0"
	"__mutex_init\0"
	"__init_waitqueue_head\0"
	"alloc_chrdev_region\0"
	"kfree\0"
	"cdev_init\0"
	"cdev_add\0"
	"class_create\0"
	"device_create\0"
	"proc_create\0"
	"kernel_kobj\0"
	"kobject_create_and_add\0"
	"sysfs_create_file_ns\0"
	"kthread_create_on_node\0"
	"wake_up_process\0"
	"device_destroy\0"
	"class_destroy\0"
	"cdev_del\0"
	"unregister_chrdev_region\0"
	"proc_remove\0"
	"sysfs_remove_file_ns\0"
	"kobject_put\0"
	"kthread_stop\0"
	"__x86_indirect_thunk_rax\0"
	"seq_read\0"
	"seq_lseek\0"
	"single_release\0"
	"__fentry__\0"
	"_printk\0"
	"__x86_return_thunk\0"
	"__ref_stack_chk_guard\0"
	"msleep\0"
	"kthread_should_stop\0"
	"get_random_bytes\0"
	"mutex_lock\0"
	"__wake_up\0"
	"mutex_unlock\0"
	"__stack_chk_fail\0"
	"kstrtoint\0"
	"sprintf\0"
	"single_open\0"
	"seq_printf\0"
	"validate_usercopy_range\0"
	"__check_object_size\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "C7378BED2E26A5C025CC83F");
