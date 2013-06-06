#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/rtc.h>
#include <linux/platform_device.h>

struct platform01_platform_data {
    int test;
};

static struct platform01_platform_data my_platform_data = {
 .test    = 100,
};

static int __devexit platform01_remove(struct platform_device *dev)
{
    struct platform01_platform_data *data;

    printk(KERN_INFO "platform01_remove():\n");

    data=platform_get_drvdata(dev);
    if(data!=NULL)
        printk(KERN_INFO " - data.test=%d\n", data->test);
    else
        printk(KERN_INFO " - data in NULL\n");
    
    return 0;
}

static int __devinit platform01_probe(struct platform_device *dev) 
{
    printk(KERN_INFO "platform01_probe():\n");
    printk(KERN_INFO " - name: %s, id: %d, n_res: %d \n", 
           dev->name, dev->id, dev->num_resources);

    platform_set_drvdata(dev, &my_platform_data);
    return 0;
}

static void __devexit platform01_device_release(struct device *dev) {

}

static struct platform_driver platform01_platform_driver = {
    .driver = {
        .name = "platform01",
        .owner = THIS_MODULE,
    },
    .probe  = platform01_probe,
    .remove = __devexit_p(platform01_remove),
};

static struct platform_device platform01_platform_device = {
    .name = "platform01",
    .id   = 0,
    .dev = {
        .platform_data = &my_platform_data,
        .release = __devexit_p(platform01_device_release),
    }
};

static int __init platform01_init(void)
{
    int err;

    printk(KERN_INFO "platform01_init()\n");
    
    printk(KERN_INFO "platform_driver_register()\n");
    err = platform_driver_register(&platform01_platform_driver);
    if (err)
        return err;

    printk(KERN_INFO "platform_device_add()\n");
    err = platform_device_register(&platform01_platform_device);
    if (!err)
        return 0;

    platform_device_put(&platform01_platform_device);
    platform_driver_unregister(&platform01_platform_driver);
    return err;
}

static void __exit platform01_exit(void)
{
    printk(KERN_INFO "platform01_exit()\n");

    platform_device_unregister(&platform01_platform_device);
    platform_driver_unregister(&platform01_platform_driver);
}

module_init(platform01_init);
module_exit(platform01_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Krzysztof Adamski <k@japko.eu>");
MODULE_DESCRIPTION("platform01 test driver");
