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
	{ 0xd272d446, "__stack_chk_fail" },
	{ 0xdd6830c7, "sprintf" },
	{ 0x6464d17c, "kernel_kobj" },
	{ 0xed8368be, "kobject_create_and_add" },
	{ 0xbbf19d39, "sysfs_create_file_ns" },
	{ 0x98b39dbb, "kobject_put" },
	{ 0x2cc2c689, "sysfs_remove_file_ns" },
	{ 0xd272d446, "__fentry__" },
	{ 0xbd03ed67, "__ref_stack_chk_guard" },
	{ 0xd09b06f5, "kstrtoint" },
	{ 0xe8213e80, "_printk" },
	{ 0x814e12e5, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0xd272d446,
	0xd272d446,
	0xdd6830c7,
	0x6464d17c,
	0xed8368be,
	0xbbf19d39,
	0x98b39dbb,
	0x2cc2c689,
	0xd272d446,
	0xbd03ed67,
	0xd09b06f5,
	0xe8213e80,
	0x814e12e5,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"__x86_return_thunk\0"
	"__stack_chk_fail\0"
	"sprintf\0"
	"kernel_kobj\0"
	"kobject_create_and_add\0"
	"sysfs_create_file_ns\0"
	"kobject_put\0"
	"sysfs_remove_file_ns\0"
	"__fentry__\0"
	"__ref_stack_chk_guard\0"
	"kstrtoint\0"
	"_printk\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "ACF9BCD5A962B952ED2C290");
