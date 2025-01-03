#include <linux/module.h>
#include <linux/init.h>
#include <linux/tty.h>
#include <linux/kd.h>
#include <linux/console_struct.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/sysfs.h>

MODULE_DESCRIPTION("Keyboard LED Firfe module 3 task");
MODULE_AUTHOR("serov");
MODULE_LICENSE("GPL");

static struct tty_driver* kbled_driver;
static char led_status = 0;

#define ALL_LEDS_OFF  0x00

static ssize_t kbled_write(struct kobject* kobj, struct kobj_attribute* attr,
    const char* buf, size_t count)
{
    int new_value;
    if (kstrtoint(buf, 10, &new_value))
        return -EINVAL;

    if (new_value < 0 || new_value > 7)
        return -EINVAL;

    led_status = new_value;
    (kbled_driver->ioctl)(vc_cons[fg_console].d->vc_tty, NULL, KDSETLED, led_status);

    return count;
}

static struct kobj_attribute kbled_attr = __ATTR(led_control, 0664, NULL, kbled_write);

static int __init kbleds_init(void)
{
    int ret;

    kbled_driver = vc_cons[fg_console].d->vc_tty->driver;

    ret = sysfs_create_file(kernel_kobj, &kbled_attr.attr);
    if (ret)
        printk(KERN_ERR "Failed to create sysfs attribute\n");
    else
        printk(KERN_INFO "kbleds: sysfs interface created\n");

    return ret;
}

static void __exit kbleds_exit(void)
{
    sysfs_remove_file(kernel_kobj, &kbled_attr.attr);
    (kbled_driver->ioctl)(vc_cons[fg_console].d->vc_tty, NULL, KDSETLED, ALL_LEDS_OFF);
    printk(KERN_INFO "kbleds: module unloaded\n");
}

module_init(kbleds_init);
module_exit(kbleds_exit);
