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
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x92997ed8, "_printk" },
	{ 0x416a268, "gpio_to_desc" },
	{ 0x4f58c2e, "gpiod_set_raw_value" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0xae353d77, "arm_copy_from_user" },
	{ 0x7682ba4e, "__copy_overflow" },
	{ 0x5f754e5a, "memset" },
	{ 0x3ea1b6e4, "__stack_chk_fail" },
	{ 0xaa163239, "gpiod_get_raw_value" },
	{ 0x51a910c0, "arm_copy_to_user" },
	{ 0x800473f, "__cond_resched" },
	{ 0xfe487975, "init_wait_entry" },
	{ 0x647af474, "prepare_to_wait_event" },
	{ 0x8ddd8aad, "schedule_timeout" },
	{ 0x49970de8, "finish_wait" },
	{ 0x126b4b23, "__register_chrdev" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0x47229b5c, "gpio_request" },
	{ 0xa87371c, "gpiod_direction_output_raw" },
	{ 0x9a6a51fd, "gpiod_direction_input" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0xfe990052, "gpio_free" },
	{ 0x8a1d9cf5, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "AFE51F2E9FC2ACCCCCAB5FB");
