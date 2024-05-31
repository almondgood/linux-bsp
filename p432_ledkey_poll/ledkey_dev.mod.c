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
	{ 0x48f39e6f, "module_put" },
	{ 0xc1514a3b, "free_irq" },
	{ 0xfe990052, "gpio_free" },
	{ 0x37a0cba, "kfree" },
	{ 0x637493f3, "__wake_up" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0x76a108e5, "kmalloc_caches" },
	{ 0x15f0cc76, "kmalloc_trace" },
	{ 0x5b6faea8, "try_module_get" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0x47229b5c, "gpio_request" },
	{ 0x416a268, "gpio_to_desc" },
	{ 0xa87371c, "gpiod_direction_output_raw" },
	{ 0x9a6a51fd, "gpiod_direction_input" },
	{ 0x52cf0d, "gpiod_to_irq" },
	{ 0x92d5838e, "request_threaded_irq" },
	{ 0x3ea1b6e4, "__stack_chk_fail" },
	{ 0x28118cb6, "__get_user_1" },
	{ 0x4f58c2e, "gpiod_set_raw_value" },
	{ 0xbb72d4fe, "__put_user_1" },
	{ 0x800473f, "__cond_resched" },
	{ 0xfe487975, "init_wait_entry" },
	{ 0x1000e51, "schedule" },
	{ 0x647af474, "prepare_to_wait_event" },
	{ 0x49970de8, "finish_wait" },
	{ 0x126b4b23, "__register_chrdev" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0x8a1d9cf5, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "86C11FE3A285F39FEF0E51E");
