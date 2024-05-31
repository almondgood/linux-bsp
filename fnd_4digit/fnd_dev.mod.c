#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
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

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif


static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
	{ 0x8e865d3c, "arm_delay_ops" },
	{ 0x416a268, "gpio_to_desc" },
	{ 0x4f58c2e, "gpiod_set_raw_value" },
	{ 0xb3f7646e, "kthread_should_stop" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x47229b5c, "gpio_request" },
	{ 0xa87371c, "gpiod_direction_output_raw" },
	{ 0x7adf5d10, "kthread_create_on_node" },
	{ 0x3aae7090, "wake_up_process" },
	{ 0x5250063a, "misc_register" },
	{ 0x92997ed8, "_printk" },
	{ 0x8ad5c6ba, "kthread_stop" },
	{ 0xfe990052, "gpio_free" },
	{ 0x650c9864, "misc_deregister" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0xc358aaf8, "snprintf" },
	{ 0xae353d77, "arm_copy_from_user" },
	{ 0x7682ba4e, "__copy_overflow" },
	{ 0x5f754e5a, "memset" },
	{ 0x3ea1b6e4, "__stack_chk_fail" },
	{ 0x8a1d9cf5, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "D2FAA8CC20DE83814E5FED0");
