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
	{ 0x546c19d9, "validate_usercopy_range" },
	{ 0xa61fd7aa, "__check_object_size" },
	{ 0x092a35a2, "_copy_from_user" },
	{ 0x90a48d82, "__ubsan_handle_out_of_bounds" },
	{ 0xd272d446, "__stack_chk_fail" },
	{ 0x40a621c5, "snprintf" },
	{ 0x437e81c7, "simple_read_from_buffer" },
	{ 0x92878b20, "proc_create" },
	{ 0x6464d17c, "kernel_kobj" },
	{ 0xed8368be, "kobject_create_and_add" },
	{ 0xbbf19d39, "sysfs_create_file_ns" },
	{ 0x98b39dbb, "kobject_put" },
	{ 0x54216a7d, "proc_remove" },
	{ 0x2cc2c689, "sysfs_remove_file_ns" },
	{ 0x4d8419c6, "param_ops_int" },
	{ 0xd272d446, "__fentry__" },
	{ 0xd09b06f5, "kstrtoint" },
	{ 0xe8213e80, "_printk" },
	{ 0xd272d446, "__x86_return_thunk" },
	{ 0xdd6830c7, "sprintf" },
	{ 0xbd03ed67, "__ref_stack_chk_guard" },
	{ 0x814e12e5, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0x546c19d9,
	0xa61fd7aa,
	0x092a35a2,
	0x90a48d82,
	0xd272d446,
	0x40a621c5,
	0x437e81c7,
	0x92878b20,
	0x6464d17c,
	0xed8368be,
	0xbbf19d39,
	0x98b39dbb,
	0x54216a7d,
	0x2cc2c689,
	0x4d8419c6,
	0xd272d446,
	0xd09b06f5,
	0xe8213e80,
	0xd272d446,
	0xdd6830c7,
	0xbd03ed67,
	0x814e12e5,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"validate_usercopy_range\0"
	"__check_object_size\0"
	"_copy_from_user\0"
	"__ubsan_handle_out_of_bounds\0"
	"__stack_chk_fail\0"
	"snprintf\0"
	"simple_read_from_buffer\0"
	"proc_create\0"
	"kernel_kobj\0"
	"kobject_create_and_add\0"
	"sysfs_create_file_ns\0"
	"kobject_put\0"
	"proc_remove\0"
	"sysfs_remove_file_ns\0"
	"param_ops_int\0"
	"__fentry__\0"
	"kstrtoint\0"
	"_printk\0"
	"__x86_return_thunk\0"
	"sprintf\0"
	"__ref_stack_chk_guard\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "DD8524744A094C0FBE2AD83");
