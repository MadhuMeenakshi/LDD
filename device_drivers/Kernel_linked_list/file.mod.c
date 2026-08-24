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
	{ 0xb6c08e4c, "class_create" },
	{ 0xf350d701, "device_create" },
	{ 0x0bc5fb0d, "unregister_chrdev_region" },
	{ 0x0c92f06e, "cdev_del" },
	{ 0xfd4b4a36, "class_destroy" },
	{ 0xbd03ed67, "random_kmalloc_seed" },
	{ 0x08bfc903, "kmalloc_caches" },
	{ 0xecd17989, "__kmalloc_cache_noprof" },
	{ 0x546c19d9, "validate_usercopy_range" },
	{ 0xa61fd7aa, "__check_object_size" },
	{ 0x092a35a2, "_copy_from_user" },
	{ 0xcb8b6ec6, "kfree" },
	{ 0x90a48d82, "__ubsan_handle_out_of_bounds" },
	{ 0xbd03ed67, "__ref_stack_chk_guard" },
	{ 0x40a621c5, "scnprintf" },
	{ 0x092a35a2, "_copy_to_user" },
	{ 0xf64ac983, "__copy_overflow" },
	{ 0xd272d446, "__stack_chk_fail" },
	{ 0x30a11079, "device_destroy" },
	{ 0xd272d446, "__fentry__" },
	{ 0xe8213e80, "_printk" },
	{ 0xd272d446, "__x86_return_thunk" },
	{ 0x9f222e1e, "alloc_chrdev_region" },
	{ 0x4c075f7d, "cdev_init" },
	{ 0x6459621a, "cdev_add" },
	{ 0x814e12e5, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0xb6c08e4c,
	0xf350d701,
	0x0bc5fb0d,
	0x0c92f06e,
	0xfd4b4a36,
	0xbd03ed67,
	0x08bfc903,
	0xecd17989,
	0x546c19d9,
	0xa61fd7aa,
	0x092a35a2,
	0xcb8b6ec6,
	0x90a48d82,
	0xbd03ed67,
	0x40a621c5,
	0x092a35a2,
	0xf64ac983,
	0xd272d446,
	0x30a11079,
	0xd272d446,
	0xe8213e80,
	0xd272d446,
	0x9f222e1e,
	0x4c075f7d,
	0x6459621a,
	0x814e12e5,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"class_create\0"
	"device_create\0"
	"unregister_chrdev_region\0"
	"cdev_del\0"
	"class_destroy\0"
	"random_kmalloc_seed\0"
	"kmalloc_caches\0"
	"__kmalloc_cache_noprof\0"
	"validate_usercopy_range\0"
	"__check_object_size\0"
	"_copy_from_user\0"
	"kfree\0"
	"__ubsan_handle_out_of_bounds\0"
	"__ref_stack_chk_guard\0"
	"scnprintf\0"
	"_copy_to_user\0"
	"__copy_overflow\0"
	"__stack_chk_fail\0"
	"device_destroy\0"
	"__fentry__\0"
	"_printk\0"
	"__x86_return_thunk\0"
	"alloc_chrdev_region\0"
	"cdev_init\0"
	"cdev_add\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "CC411D652487B1EA548ECA4");
