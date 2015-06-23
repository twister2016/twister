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
	{ 0x7dc216d9, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x35b6b772, __VMLINUX_SYMBOL_STR(param_ops_charp) },
	{ 0x11602790, __VMLINUX_SYMBOL_STR(pci_unregister_driver) },
	{ 0x560696, __VMLINUX_SYMBOL_STR(__pci_register_driver) },
	{ 0xe2d5255a, __VMLINUX_SYMBOL_STR(strcmp) },
	{ 0xbe2ac387, __VMLINUX_SYMBOL_STR(__dynamic_dev_dbg) },
	{ 0x197d09a0, __VMLINUX_SYMBOL_STR(_dev_info) },
	{ 0x30aebbe2, __VMLINUX_SYMBOL_STR(dev_notice) },
	{ 0x2964e8eb, __VMLINUX_SYMBOL_STR(pci_intx_mask_supported) },
	{ 0x26cc4d58, __VMLINUX_SYMBOL_STR(__uio_register_device) },
	{ 0xb528bf1a, __VMLINUX_SYMBOL_STR(sysfs_create_group) },
	{ 0x57f37fc7, __VMLINUX_SYMBOL_STR(pci_enable_msix) },
	{ 0xcfc62f27, __VMLINUX_SYMBOL_STR(xen_start_info) },
	{ 0x731dba7a, __VMLINUX_SYMBOL_STR(xen_domain_type) },
	{ 0xcc151d19, __VMLINUX_SYMBOL_STR(dma_supported) },
	{ 0x42c8de35, __VMLINUX_SYMBOL_STR(ioremap_nocache) },
	{ 0x4b3c4477, __VMLINUX_SYMBOL_STR(dma_set_mask) },
	{ 0x790f1b45, __VMLINUX_SYMBOL_STR(pci_set_master) },
	{ 0x63e35191, __VMLINUX_SYMBOL_STR(dev_err) },
	{ 0xc7d19ce3, __VMLINUX_SYMBOL_STR(pci_request_regions) },
	{ 0x503ba93e, __VMLINUX_SYMBOL_STR(pci_enable_device) },
	{ 0xa6327d27, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0xdf08d9a5, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x5e074b5a, __VMLINUX_SYMBOL_STR(pci_check_and_mask_intx) },
	{ 0x17b41986, __VMLINUX_SYMBOL_STR(pci_intx) },
	{ 0x46a37692, __VMLINUX_SYMBOL_STR(pci_cfg_access_unlock) },
	{ 0xb01fc3d6, __VMLINUX_SYMBOL_STR(pci_cfg_access_lock) },
	{ 0xcfc3ee1f, __VMLINUX_SYMBOL_STR(remap_pfn_range) },
	{ 0xc715d9e0, __VMLINUX_SYMBOL_STR(boot_cpu_data) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x918d51ec, __VMLINUX_SYMBOL_STR(pci_disable_msix) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x346c7121, __VMLINUX_SYMBOL_STR(dev_set_drvdata) },
	{ 0x3a426df2, __VMLINUX_SYMBOL_STR(pci_disable_device) },
	{ 0x1e6b569b, __VMLINUX_SYMBOL_STR(pci_release_regions) },
	{ 0x10102dba, __VMLINUX_SYMBOL_STR(uio_unregister_device) },
	{ 0x4057ce2, __VMLINUX_SYMBOL_STR(sysfs_remove_group) },
	{ 0x44d787a, __VMLINUX_SYMBOL_STR(dev_get_drvdata) },
	{ 0xedc03953, __VMLINUX_SYMBOL_STR(iounmap) },
	{ 0x28318305, __VMLINUX_SYMBOL_STR(snprintf) },
	{ 0x43050f83, __VMLINUX_SYMBOL_STR(pci_enable_sriov) },
	{ 0x32519f11, __VMLINUX_SYMBOL_STR(pci_num_vf) },
	{ 0xab5240b2, __VMLINUX_SYMBOL_STR(pci_disable_sriov) },
	{ 0x3c80c06c, __VMLINUX_SYMBOL_STR(kstrtoull) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=uio";


MODULE_INFO(srcversion, "1615B72D1C74E5105B464A1");
