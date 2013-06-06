#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/rtc.h>
#include <linux/platform_device.h>

static struct platform_device *platform02_platform_device;

struct platform02_platform_data {
    int test;
};

static struct platform02_platform_data my_platform_data = {
 .test    = 100,
};

static int __devexit platform02_remove(struct platform_device *dev)
{
    struct platform02_platform_data *data;

    printk(KERN_INFO "platform02_remove():\n");

    data=platform_get_drvdata(dev);
    if(data!=NULL)
        printk(KERN_INFO " - data.test=%d\n", data->test);
    else
        printk(KERN_INFO " - data in NULL\n");
    
    return 0;
}

static int __devinit platform02_probe(struct platform_device *dev) 
{
    printk(KERN_INFO "platform02_probe():\n");
    printk(KERN_INFO " - name: %s, id: %d, n_res: %d \n", 
           dev->name, dev->id, dev->num_resources);

    platform_set_drvdata(dev, &my_platform_data);
    return 0;
}


static struct platform_driver platform02_platform_driver = {
    .driver = {
        .name = "platform02",
        .owner = THIS_MODULE,
    },
    .probe  = platform02_probe,
    .remove = __devexit_p(platform02_remove),
};

static int __init platform02_init(void)
{
    int err;

    printk(KERN_INFO "platform02_init()\n");
    
    printk(KERN_INFO "platform_driver_register()\n");
    err = platform_driver_register(&platform02_platform_driver);
    if (err)
        return err;

    printk(KERN_INFO "platform_device_alloc()\n");
    platform02_platform_device = platform_device_alloc("platform02", 0);
    if (platform02_platform_device == NULL) {
        err = -ENOMEM;
        goto exit_driver_unregister;
    }

    printk(KERN_INFO "platform_device_add()\n");
    err = platform_device_add(platform02_platform_device);
    if (err)
        goto exit_device_put;

    return 0;

exit_device_put:
    platform_device_put(platform02_platform_device);

exit_driver_unregister:
    platform_driver_unregister(&platform02_platform_driver);
    return err;

}

static void __exit platform02_exit(void)
{
    printk(KERN_INFO "platform02_exit()\n");

    printk(KERN_INFO "platform_device_unregister()\n");
    platform_device_unregister(platform02_platform_device);
    printk(KERN_INFO "platform_driver_unregister()\n");
    platform_driver_unregister(&platform02_platform_driver);
}

module_init(platform02_init);
module_exit(platform02_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Krzysztof Adamski <k@japko.eu>");
MODULE_DESCRIPTION("platform02 test driver");
