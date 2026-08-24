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
	{ 0xb795e751, "__spi_register_driver" },
	{ 0xbd03ed67, "__ref_stack_chk_guard" },
	{ 0x7659839c, "spi_sync" },
	{ 0xd272d446, "__stack_chk_fail" },
	{ 0x6b62c318, "driver_unregister" },
	{ 0xd272d446, "__fentry__" },
	{ 0xe8213e80, "_printk" },
	{ 0xd272d446, "__x86_return_thunk" },
	{ 0x814e12e5, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0xb795e751,
	0xbd03ed67,
	0x7659839c,
	0xd272d446,
	0x6b62c318,
	0xd272d446,
	0xe8213e80,
	0xd272d446,
	0x814e12e5,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"__spi_register_driver\0"
	"__ref_stack_chk_guard\0"
	"spi_sync\0"
	"__stack_chk_fail\0"
	"driver_unregister\0"
	"__fentry__\0"
	"_printk\0"
	"__x86_return_thunk\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");

MODULE_ALIAS("of:N*T*Cmadhu,dummy_spi");
MODULE_ALIAS("of:N*T*Cmadhu,dummy_spiC*");
MODULE_ALIAS("spi:dummy_spi");

MODULE_INFO(srcversion, "92B6569F1C7AA493F30E28E");
