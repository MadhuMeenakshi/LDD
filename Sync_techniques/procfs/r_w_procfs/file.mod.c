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
	{ 0xe8213e80, "_printk" },
	{ 0xd272d446, "__x86_return_thunk" },
	{ 0x90a48d82, "__ubsan_handle_out_of_bounds" },
	{ 0x437e81c7, "simple_read_from_buffer" },
	{ 0x9479a1e8, "strnlen" },
	{ 0x92878b20, "proc_create" },
	{ 0xe54e0a6b, "__fortify_panic" },
	{ 0x54216a7d, "proc_remove" },
	{ 0xd272d446, "__fentry__" },
	{ 0x546c19d9, "validate_usercopy_range" },
	{ 0xa61fd7aa, "__check_object_size" },
	{ 0x092a35a2, "_copy_from_user" },
	{ 0x814e12e5, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0xe8213e80,
	0xd272d446,
	0x90a48d82,
	0x437e81c7,
	0x9479a1e8,
	0x92878b20,
	0xe54e0a6b,
	0x54216a7d,
	0xd272d446,
	0x546c19d9,
	0xa61fd7aa,
	0x092a35a2,
	0x814e12e5,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"_printk\0"
	"__x86_return_thunk\0"
	"__ubsan_handle_out_of_bounds\0"
	"simple_read_from_buffer\0"
	"strnlen\0"
	"proc_create\0"
	"__fortify_panic\0"
	"proc_remove\0"
	"__fentry__\0"
	"validate_usercopy_range\0"
	"__check_object_size\0"
	"_copy_from_user\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "2CF68D0A9913A69207D9B6B");
