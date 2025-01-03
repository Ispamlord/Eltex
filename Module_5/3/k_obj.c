#include <linux/module.h>
#include <linux/printk.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/tty.h>
#include <linux/kd.h>
#include <linux/vt.h>
#include <linux/console_struct.h>
#include <linux/timer.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Keyboard LED control via sysfs");

static struct kobject* example_kobject;
static int led_mask = 0;
static struct tty_driver* my_driver;
static struct timer_list my_timer;

#define BLINK_DELAY HZ / 5
#define RESTORE_LEDS 0xFF

static void update_leds(int mask)
{
    if (vc_cons[0].d && my_driver && my_driver->ops && my_driver->ops->ioctl) {
        (my_driver->ops->ioctl)(vc_cons[0].d->port.tty, KDSETLED, mask);
    }
    else {
        pr_warn("kbleds: Failed to update LEDs, tty or driver not available\n");
    }
}

static ssize_t led_show(struct kobject* kobj, struct kobj_attribute* attr, char* buf)
{
    return sprintf(buf, "%d\n", led_mask);
}

static ssize_t led_store(struct kobject* kobj, struct kobj_attribute* attr, const char* buf, size_t count)
{
    int ret = sscanf(buf, "%du", &led_mask);
    if (ret != 1) {
        pr_warn("kbleds: Invalid input\n");
        return -EINVAL;
    }

    update_leds(led_mask);
    return count;
}

static struct kobj_attribute led_attribute = __ATTR(test, 0660, led_show, led_store);

static void my_timer_func(struct timer_list* ptr)
{
    static int state = 0;
    state = state ? 0 : led_mask;
    update_leds(state);
    mod_timer(&my_timer, jiffies + BLINK_DELAY);
}

static int __init sys_init(void)
{
    int error;
    pr_info("kbleds: initializing sysfs LED control\n");

    example_kobject = kobject_create_and_add("kbleds", kernel_kobj);
    if (!example_kobject)
        return -ENOMEM;

    error = sysfs_create_file(example_kobject, &led_attribute.attr);
    if (error) {
        pr_err("kbleds: failed to create sysfs entry\n");
        kobject_put(example_kobject);
        return error;
    }

    if (!vc_cons[0].d) {
        pr_err("kbleds: No active console available\n");
        kobject_put(example_kobject);
        return -ENODEV;
    }

    my_driver = vc_cons[0].d->port.tty->driver;
    if (!my_driver) {
        pr_err("kbleds: Failed to get tty driver\n");
        kobject_put(example_kobject);
        return -ENODEV;
    }

    timer_setup(&my_timer, my_timer_func, 0);
    my_timer.expires = jiffies + BLINK_DELAY;
    add_timer(&my_timer);

    return 0;
}

static void __exit sys_exit(void)
{
    pr_info("kbleds: exiting and cleaning up\n");
    del_timer(&my_timer);
    update_leds(RESTORE_LEDS);
    kobject_put(example_kobject);
}

module_init(sys_init);
module_exit(sys_exit);
