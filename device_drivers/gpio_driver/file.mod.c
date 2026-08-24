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
	{ 0xd272d446, "__x86_return_thunk" },
	{ 0x6a5c12c0, "gpiod_set_value" },
	{ 0x7acf5086, "gpiod_put" },
	{ 0x05050234, "gpiod_get" },
	{ 0xfeeac846, "gpiod_get_value" },
	{ 0xea97e6f1, "platform_driver_unregister" },
	{ 0xd272d446, "__fentry__" },
	{ 0xe8213e80, "_printk" },
	{ 0xacbfeb0e, "__platform_driver_register" },
	{ 0x814e12e5, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0xd272d446,
	0x6a5c12c0,
	0x7acf5086,
	0x05050234,
	0xfeeac846,
	0xea97e6f1,
	0xd272d446,
	0xe8213e80,
	0xacbfeb0e,
	0x814e12e5,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"__x86_return_thunk\0"
	"gpiod_set_value\0"
	"gpiod_put\0"
	"gpiod_get\0"
	"gpiod_get_value\0"
	"platform_driver_unregister\0"
	"__fentry__\0"
	"_printk\0"
	"__platform_driver_register\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");

MODULE_ALIAS("of:N*T*Cmadhu,gpio-demo");
MODULE_ALIAS("of:N*T*Cmadhu,gpio-demoC*");

MODULE_INFO(srcversion, "4DD1B0337C2EC322A46F317");
