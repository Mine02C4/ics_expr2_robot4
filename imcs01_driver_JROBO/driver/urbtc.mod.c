#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x9546d51a, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x7c8869a1, __VMLINUX_SYMBOL_STR(usb_deregister) },
	{ 0xfdcf15a8, __VMLINUX_SYMBOL_STR(usb_register_driver) },
	{ 0xfbc74f64, __VMLINUX_SYMBOL_STR(__copy_from_user) },
	{ 0x929cb6, __VMLINUX_SYMBOL_STR(usb_alloc_coherent) },
	{ 0x4fe38dbd, __VMLINUX_SYMBOL_STR(down_interruptible) },
	{ 0x8ef51045, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x2935af05, __VMLINUX_SYMBOL_STR(usb_alloc_urb) },
	{ 0x12da5bb2, __VMLINUX_SYMBOL_STR(__kmalloc) },
	{ 0x93fdd3ea, __VMLINUX_SYMBOL_STR(usb_register_dev) },
	{ 0xf8c453ba, __VMLINUX_SYMBOL_STR(usb_get_dev) },
	{ 0x63b87fc5, __VMLINUX_SYMBOL_STR(__init_waitqueue_head) },
	{ 0xfa2a45e, __VMLINUX_SYMBOL_STR(__memzero) },
	{ 0xd511a1f3, __VMLINUX_SYMBOL_STR(kmem_cache_alloc) },
	{ 0x7d619889, __VMLINUX_SYMBOL_STR(usb_kill_urb) },
	{ 0xbdf1542d, __VMLINUX_SYMBOL_STR(_dev_info) },
	{ 0x3f8a9be5, __VMLINUX_SYMBOL_STR(usb_deregister_dev) },
	{ 0x2b96151a, __VMLINUX_SYMBOL_STR(dev_set_drvdata) },
	{ 0x3a1651b3, __VMLINUX_SYMBOL_STR(usb_free_urb) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x1cbb9b, __VMLINUX_SYMBOL_STR(usb_put_dev) },
	{ 0x16305289, __VMLINUX_SYMBOL_STR(warn_slowpath_null) },
	{ 0xd2191f79, __VMLINUX_SYMBOL_STR(dev_get_drvdata) },
	{ 0xf8ba6f8a, __VMLINUX_SYMBOL_STR(usb_find_interface) },
	{ 0xc8b57c27, __VMLINUX_SYMBOL_STR(autoremove_wake_function) },
	{ 0x8893fa5d, __VMLINUX_SYMBOL_STR(finish_wait) },
	{ 0x75a17bed, __VMLINUX_SYMBOL_STR(prepare_to_wait) },
	{ 0x1000e51, __VMLINUX_SYMBOL_STR(schedule) },
	{ 0xcea899ea, __VMLINUX_SYMBOL_STR(down) },
	{ 0x7d2b9d39, __VMLINUX_SYMBOL_STR(usb_bulk_msg) },
	{ 0xff808fb4, __VMLINUX_SYMBOL_STR(usb_free_coherent) },
	{ 0xb9e52429, __VMLINUX_SYMBOL_STR(__wake_up) },
	{ 0x364b3fff, __VMLINUX_SYMBOL_STR(up) },
	{ 0x9d669763, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0x8692a004, __VMLINUX_SYMBOL_STR(down_trylock) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xefd6cf06, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr0) },
	{ 0xe7090dfc, __VMLINUX_SYMBOL_STR(usb_submit_urb) },
	{ 0x67c2fa54, __VMLINUX_SYMBOL_STR(__copy_to_user) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("usb:v08DApFC00d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0FF8p0001d*dc*dsc*dp*ic*isc*ip*in*");

MODULE_INFO(srcversion, "3EB956F3CE48B57C8DC9BEF");
