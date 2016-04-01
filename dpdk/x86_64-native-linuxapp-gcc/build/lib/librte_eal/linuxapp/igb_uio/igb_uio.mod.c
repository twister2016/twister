#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
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
	{ 0x9412fa01, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x5d41c87c, __VMLINUX_SYMBOL_STR(param_ops_charp) },
	{ 0x5604dae8, __VMLINUX_SYMBOL_STR(pci_unregister_driver) },
	{ 0x19384703, __VMLINUX_SYMBOL_STR(__pci_register_driver) },
	{ 0xe2d5255a, __VMLINUX_SYMBOL_STR(strcmp) },
	{ 0xfa0d5760, __VMLINUX_SYMBOL_STR(__dynamic_dev_dbg) },
	{ 0x853eb0f0, __VMLINUX_SYMBOL_STR(_dev_info) },
	{ 0xbb8d0711, __VMLINUX_SYMBOL_STR(dev_notice) },
	{ 0xa87ad6a4, __VMLINUX_SYMBOL_STR(pci_intx_mask_supported) },
	{ 0x7df02fbe, __VMLINUX_SYMBOL_STR(__uio_register_device) },
	{ 0xb2711b8f, __VMLINUX_SYMBOL_STR(sysfs_create_group) },
	{ 0x8da84edd, __VMLINUX_SYMBOL_STR(pci_enable_msix) },
	{ 0xcfc62f27, __VMLINUX_SYMBOL_STR(xen_start_info) },
	{ 0x731dba7a, __VMLINUX_SYMBOL_STR(xen_domain_type) },
	{ 0xe605e7f1, __VMLINUX_SYMBOL_STR(dma_supported) },
	{ 0x42c8de35, __VMLINUX_SYMBOL_STR(ioremap_nocache) },
	{ 0xc384c80f, __VMLINUX_SYMBOL_STR(dma_set_mask) },
	{ 0xd6068ed2, __VMLINUX_SYMBOL_STR(pci_set_master) },
	{ 0x57a6ac32, __VMLINUX_SYMBOL_STR(dev_err) },
	{ 0x4f3045ec, __VMLINUX_SYMBOL_STR(pci_request_regions) },
	{ 0x85065003, __VMLINUX_SYMBOL_STR(pci_enable_device) },
	{ 0x6e938e79, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0x6d0fc37d, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0xa10eacdd, __VMLINUX_SYMBOL_STR(pci_check_and_mask_intx) },
	{ 0x2e07dda2, __VMLINUX_SYMBOL_STR(pci_intx) },
	{ 0x7926384b, __VMLINUX_SYMBOL_STR(pci_cfg_access_unlock) },
	{ 0xf18baf84, __VMLINUX_SYMBOL_STR(pci_cfg_access_lock) },
	{ 0x6589780f, __VMLINUX_SYMBOL_STR(remap_pfn_range) },
	{ 0xc715d9e0, __VMLINUX_SYMBOL_STR(boot_cpu_data) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x23787eb, __VMLINUX_SYMBOL_STR(pci_disable_msix) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x5fe56825, __VMLINUX_SYMBOL_STR(dev_set_drvdata) },
	{ 0x4272d0d4, __VMLINUX_SYMBOL_STR(pci_disable_device) },
	{ 0x4ef8fada, __VMLINUX_SYMBOL_STR(pci_release_regions) },
	{ 0xf038e119, __VMLINUX_SYMBOL_STR(uio_unregister_device) },
	{ 0x8a662e1b, __VMLINUX_SYMBOL_STR(sysfs_remove_group) },
	{ 0xb749ae89, __VMLINUX_SYMBOL_STR(dev_get_drvdata) },
	{ 0xedc03953, __VMLINUX_SYMBOL_STR(iounmap) },
	{ 0x28318305, __VMLINUX_SYMBOL_STR(snprintf) },
	{ 0xb9021285, __VMLINUX_SYMBOL_STR(pci_bus_type) },
	{ 0xb439ab6f, __VMLINUX_SYMBOL_STR(pci_enable_sriov) },
	{ 0x522ad8f6, __VMLINUX_SYMBOL_STR(pci_num_vf) },
	{ 0x1ab49e22, __VMLINUX_SYMBOL_STR(pci_disable_sriov) },
	{ 0x3c80c06c, __VMLINUX_SYMBOL_STR(kstrtoull) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=uio";


MODULE_INFO(srcversion, "105BA558E441D2370738318");
