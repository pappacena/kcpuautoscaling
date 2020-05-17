#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thiago F. Pappacena");
MODULE_DESCRIPTION("Module to automatically enable/disable cores of the system depending on the load.");
MODULE_VERSION("0.1.0");

static int __init cpuautoscaling_init(void) {
    printk(KERN_INFO "Hello, World!\n");
    return 0;
}
static void __exit cpuautoscaling_exit(void) {
    printk(KERN_INFO "Goodbye, World!\n");
}

module_init(cpuautoscaling_init);
module_exit(cpuautoscaling_exit);
