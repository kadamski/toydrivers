#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/rtc.h>
#include <linux/platform_device.h>

static struct platform_device *rtc01_platform_device;

static int rtc01_get_time(struct device *dev, struct rtc_time *t)
{
    t->tm_sec = 7;
    t->tm_min = 3;
    t->tm_hour = 3;
    t->tm_mday = 1;
    t->tm_mon = 6;
    t->tm_year = 113;
    t->tm_wday = 0;
    t->tm_yday = 0;
    t->tm_isdst = 0;

    if (rtc_valid_tm(t) < 0)
        dev_warn(dev, "Read invalid date/time from RTC\n");

    return 0;
}

static int rtc01_set_time(struct device *dev, struct rtc_time *t)
{
    printk(KERN_INFO "rtc01_set_time()\n");

    if (rtc_valid_tm(t) < 0) {
        dev_warn(dev, "Invalid date/time from RTC\n");
        return -EINVAL;
    }

    printk(KERN_INFO " - %d/%d/%d %d:%d:%d\n", t->tm_year+1900, t->tm_mon,
                                               t->tm_mday, t->tm_hour, t->tm_min, 
                                               t->tm_sec);
    
    return 0;
}

static int rtc01_read_alarm(struct device *dev, struct rtc_wkalrm *t)
{
    printk(KERN_INFO "rtc01_read_alarm()\n");
    return 0;
}

static int rtc01_set_alarm(struct device *dev, struct rtc_wkalrm *t)
{
    printk(KERN_INFO "rtc01_set_alarm()\n");
    return 0;
}

static int rtc01_alarm_irq_enable(struct device *dev, unsigned int enabled)
{
    printk(KERN_INFO "rtc01_irq_enable()\n");
    return 0;
}

static const struct rtc_class_ops rtc01_rtc_ops = {
        .read_time      = rtc01_get_time,
        .set_time       = rtc01_set_time,
        .read_alarm     = rtc01_read_alarm,
        .set_alarm      = rtc01_set_alarm,
        .alarm_irq_enable = rtc01_alarm_irq_enable,
};

static int __devexit rtc01_remove(struct platform_device *dev)
{
    rtc_device_unregister(platform_get_drvdata(dev));
    return 0;
}

static int __devinit rtc01_probe(struct platform_device *dev) 
{
    struct rtc_device *rtc;

    rtc = rtc_device_register("rtc-01", &dev->dev, &rtc01_rtc_ops, THIS_MODULE);
    if (IS_ERR(rtc))
        return PTR_ERR(rtc);

    platform_set_drvdata(dev, rtc);
    return 0;
}

static struct platform_driver rtc01_platform_driver = {
    .driver = {
        .name = "rtc-01",
        .owner = THIS_MODULE,
    },
    .probe  = rtc01_probe,
    .remove = __devexit_p(rtc01_remove),
};

static int __init rtc01_init(void)
{
    int err;
    
    err = platform_driver_register(&rtc01_platform_driver);
    if (err)
        return err;

    rtc01_platform_device = platform_device_alloc("rtc-01", 0);
    if (rtc01_platform_device == NULL) {
        err = -ENOMEM;
        goto exit_driver_unregister;
    }

    err = platform_device_add(rtc01_platform_device);
    if (err)
        goto exit_device_put;

    return 0;

exit_device_put:
    platform_device_put(rtc01_platform_device);

exit_driver_unregister:
    platform_driver_unregister(&rtc01_platform_driver);
    return err;

}

static void __exit rtc01_exit(void)
{
    platform_device_unregister(rtc01_platform_device);
    platform_driver_unregister(&rtc01_platform_driver);
}

module_init(rtc01_init);
module_exit(rtc01_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Krzysztof Adamski <k@japko.eu>");
MODULE_DESCRIPTION("rtc01 test driver");
