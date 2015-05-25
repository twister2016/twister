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
	{ 0xe37a5a9c, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x5d41c87c, __VMLINUX_SYMBOL_STR(param_ops_charp) },
	{ 0xcf2b6910, __VMLINUX_SYMBOL_STR(pci_unregister_driver) },
	{ 0xb87504ad, __VMLINUX_SYMBOL_STR(__pci_register_driver) },
	{ 0xe2d5255a, __VMLINUX_SYMBOL_STR(strcmp) },
	{ 0x8a96d402, __VMLINUX_SYMBOL_STR(__dynamic_dev_dbg) },
	{ 0xb4f0357b, __VMLINUX_SYMBOL_STR(_dev_info) },
	{ 0x12ba6b0e, __VMLINUX_SYMBOL_STR(dev_notice) },
	{ 0x52d0019f, __VMLINUX_SYMBOL_STR(pci_intx_mask_supported) },
	{ 0x59734793, __VMLINUX_SYMBOL_STR(__uio_register_device) },
	{ 0x2878c27f, __VMLINUX_SYMBOL_STR(sysfs_create_group) },
	{ 0xa0948b1a, __VMLINUX_SYMBOL_STR(pci_enable_msix) },
	{ 0xcfc62f27, __VMLINUX_SYMBOL_STR(xen_start_info) },
	{ 0x731dba7a, __VMLINUX_SYMBOL_STR(xen_domain_type) },
	{ 0xd0ffc1f4, __VMLINUX_SYMBOL_STR(dma_supported) },
	{ 0x42c8de35, __VMLINUX_SYMBOL_STR(ioremap_nocache) },
	{ 0xd0089522, __VMLINUX_SYMBOL_STR(dma_set_mask) },
	{ 0x369b7e7c, __VMLINUX_SYMBOL_STR(pci_set_master) },
	{ 0x5d48e83a, __VMLINUX_SYMBOL_STR(dev_err) },
	{ 0x5f87b957, __VMLINUX_SYMBOL_STR(pci_request_regions) },
	{ 0xa2465164, __VMLINUX_SYMBOL_STR(pci_enable_device) },
	{ 0xa8758b20, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0xf510c4, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0xe4d55c25, __VMLINUX_SYMBOL_STR(pci_check_and_mask_intx) },
	{ 0x6c353488, __VMLINUX_SYMBOL_STR(pci_intx) },
	{ 0x6b56f867, __VMLINUX_SYMBOL_STR(pci_cfg_access_unlock) },
	{ 0x4b73bf7e, __VMLINUX_SYMBOL_STR(pci_cfg_access_lock) },
	{ 0xd288944f, __VMLINUX_SYMBOL_STR(remap_pfn_range) },
	{ 0xc715d9e0, __VMLINUX_SYMBOL_STR(boot_cpu_data) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xd92ddf09, __VMLINUX_SYMBOL_STR(pci_disable_msix) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x5fe56825, __VMLINUX_SYMBOL_STR(dev_set_drvdata) },
	{ 0x659987df, __VMLINUX_SYMBOL_STR(pci_disable_device) },
	{ 0x890750, __VMLINUX_SYMBOL_STR(pci_release_regions) },
	{ 0x1c90191b, __VMLINUX_SYMBOL_STR(uio_unregister_device) },
	{ 0xa80fea41, __VMLINUX_SYMBOL_STR(sysfs_remove_group) },
	{ 0xb749ae89, __VMLINUX_SYMBOL_STR(dev_get_drvdata) },
	{ 0xedc03953, __VMLINUX_SYMBOL_STR(iounmap) },
	{ 0x28318305, __VMLINUX_SYMBOL_STR(snprintf) },
	{ 0x83894c5, __VMLINUX_SYMBOL_STR(pci_enable_sriov) },
	{ 0x3992520c, __VMLINUX_SYMBOL_STR(pci_num_vf) },
	{ 0x89dfc69c, __VMLINUX_SYMBOL_STR(pci_disable_sriov) },
	{ 0x3c80c06c, __VMLINUX_SYMBOL_STR(kstrtoull) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=uio";


MODULE_INFO(srcversion, "1615B72D1C74E5105B464A1");
