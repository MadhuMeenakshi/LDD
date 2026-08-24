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
	{ 0xf350d701, "device_create" },
	{ 0x0bc5fb0d, "unregister_chrdev_region" },
	{ 0x0c92f06e, "cdev_del" },
	{ 0xfd4b4a36, "class_destroy" },
	{ 0x546c19d9, "validate_usercopy_range" },
	{ 0xa61fd7aa, "__check_object_size" },
	{ 0x092a35a2, "_copy_to_user" },
	{ 0x2d88a3ab, "flush_work" },
	{ 0x30a11079, "device_destroy" },
	{ 0x092a35a2, "_copy_from_user" },
	{ 0xaef1f20d, "system_wq" },
	{ 0x49733ad6, "queue_work_on" },
	{ 0x90a48d82, "__ubsan_handle_out_of_bounds" },
	{ 0xd272d446, "__fentry__" },
	{ 0xe8213e80, "_printk" },
	{ 0xd272d446, "__x86_return_thunk" },
	{ 0x9f222e1e, "alloc_chrdev_region" },
	{ 0x4c075f7d, "cdev_init" },
	{ 0x6459621a, "cdev_add" },
	{ 0xb6c08e4c, "class_create" },
	{ 0x814e12e5, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0xf350d701,
	0x0bc5fb0d,
	0x0c92f06e,
	0xfd4b4a36,
	0x546c19d9,
	0xa61fd7aa,
	0x092a35a2,
	0x2d88a3ab,
	0x30a11079,
	0x092a35a2,
	0xaef1f20d,
	0x49733ad6,
	0x90a48d82,
	0xd272d446,
	0xe8213e80,
	0xd272d446,
	0x9f222e1e,
	0x4c075f7d,
	0x6459621a,
	0xb6c08e4c,
	0x814e12e5,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"device_create\0"
	"unregister_chrdev_region\0"
	"cdev_del\0"
	"class_destroy\0"
	"validate_usercopy_range\0"
	"__check_object_size\0"
	"_copy_to_user\0"
	"flush_work\0"
	"device_destroy\0"
	"_copy_from_user\0"
	"system_wq\0"
	"queue_work_on\0"
	"__ubsan_handle_out_of_bounds\0"
	"__fentry__\0"
	"_printk\0"
	"__x86_return_thunk\0"
	"alloc_chrdev_region\0"
	"cdev_init\0"
	"cdev_add\0"
	"class_create\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "D8907B3C5CB4F73F196CB77");
