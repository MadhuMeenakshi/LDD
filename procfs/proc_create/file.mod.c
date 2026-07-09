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
	{ 0xbd03ed67, "__ref_stack_chk_guard" },
	{ 0x437e81c7, "simple_read_from_buffer" },
	{ 0xd272d446, "__stack_chk_fail" },
	{ 0x54216a7d, "proc_remove" },
	{ 0xd272d446, "__fentry__" },
	{ 0x92878b20, "proc_create" },
	{ 0xe8213e80, "_printk" },
	{ 0x814e12e5, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0xd272d446,
	0xbd03ed67,
	0x437e81c7,
	0xd272d446,
	0x54216a7d,
	0xd272d446,
	0x92878b20,
	0xe8213e80,
	0x814e12e5,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"__x86_return_thunk\0"
	"__ref_stack_chk_guard\0"
	"simple_read_from_buffer\0"
	"__stack_chk_fail\0"
	"proc_remove\0"
	"__fentry__\0"
	"proc_create\0"
	"_printk\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "C4BB01BF2B42411541DC584");
