#include <linux/tty.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kd.h>
#include <linux/kobject.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/vt.h>
#include <linux/console.h>
#include <linux/timer.h>

#define BLINK_DELAY HZ / 5
#define ALL_LEDS_ON 0x07
#define RESTORE_LEDS 0xFF

static struct kobject* example_kobject;
static struct timer_list my_timer;
static struct tty_driver* my_driver;
static int kbledstatus = 0;
static int value = 0;

static ssize_t foo_show(struct kobject* kobj, struct kobj_attribute* attr, char* buf) {
    return sysfs_emit(buf, "%d\n", value);
}

static ssize_t foo_store(struct kobject* kobj, struct kobj_attribute* attr, const char* buf, size_t count) {
    int ret;
    ret = kstrtoint(buf, 10, &value);
    if (ret < 0)
        return ret;
    return count;
}

static void my_timer_func(struct timer_list* unused) {
    struct vc_data* vc = vc_cons[fg_console].d;
    struct tty_struct* t = vc->port.tty;

    if (kbledstatus == value)
        kbledstatus = RESTORE_LEDS;
    else
        kbledstatus = value;

    if (t && t->driver && t->driver->ops->ioctl)
        (t->driver->ops->ioctl)(t, KDSETLED, kbledstatus);

    mod_timer(&my_timer, jiffies + BLINK_DELAY);
}

static int kbleds_init(void) {
    struct vc_data* vc = vc_cons[fg_console].d;

    if (!vc || !vc->port.tty)
        return -ENODEV;

    my_driver = vc->port.tty->driver;

    timer_setup(&my_timer, my_timer_func, 0);
    mod_timer(&my_timer, jiffies + BLINK_DELAY);
    return 0;
}

static void kbleds_cleanup(void) {
    struct vc_data* vc = vc_cons[fg_console].d;

    del_timer_sync(&my_timer);
    if (vc && vc->port.tty && my_driver->ops->ioctl)
        (my_driver->ops->ioctl)(vc->port.tty, KDSETLED, RESTORE_LEDS);
}

static struct kobj_attribute foo_attribute = __ATTR(value, 0660, foo_show, foo_store);

static int __init led_init(void) {
    int error = 0;

    example_kobject = kobject_create_and_add("sys_test", kernel_kobj);
    if (!example_kobject)
        return -ENOMEM;

    error = sysfs_create_file(example_kobject, &foo_attribute.attr);
    if (error) {
        pr_debug("Failed to create sysfs entry\n");
        return error;
    }

    error = kbleds_init();
    if (error)
        pr_debug("Failed to initialize kbleds\n");

    return error;
}

static void __exit exit_module(void) {
    kobject_put(example_kobject);
    kbleds_cleanup();
}

MODULE_LICENSE("GPL");
module_init(led_init);
module_exit(exit_module);
