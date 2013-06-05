#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int __init rtc01_init(void)
{
    printk(KERN_INFO "Init\n");
    return 0;
}

static void __exit rtc01_exit(void)
{
    printk(KERN_INFO "Exit\n");
}

module_init(rtc01_init);
module_exit(rtc01_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Krzysztof Adamski <k@japko.eu>");
MODULE_DESCRIPTION("rtc01 test driver");
