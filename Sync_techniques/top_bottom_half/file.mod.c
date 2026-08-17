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
	{ 0x02f9bbf0, "timer_init_key" },
	{ 0x058c185a, "jiffies" },
	{ 0x32feeafc, "mod_timer" },
	{ 0x9c0551c6, "__tasklet_schedule" },
	{ 0x2352b148, "timer_delete_sync" },
	{ 0x9c0551c6, "tasklet_kill" },
	{ 0xd272d446, "__fentry__" },
	{ 0xe8213e80, "_printk" },
	{ 0xd272d446, "__x86_return_thunk" },
	{ 0xdff18c89, "tasklet_setup" },
	{ 0x814e12e5, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0x02f9bbf0,
	0x058c185a,
	0x32feeafc,
	0x9c0551c6,
	0x2352b148,
	0x9c0551c6,
	0xd272d446,
	0xe8213e80,
	0xd272d446,
	0xdff18c89,
	0x814e12e5,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"timer_init_key\0"
	"jiffies\0"
	"mod_timer\0"
	"__tasklet_schedule\0"
	"timer_delete_sync\0"
	"tasklet_kill\0"
	"__fentry__\0"
	"_printk\0"
	"__x86_return_thunk\0"
	"tasklet_setup\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "BF951CBD60FC14CB8D412E7");
