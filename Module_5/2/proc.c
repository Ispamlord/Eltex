#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define PROC_NAME "proc_example"

static char* msg;
static int len, temp;

ssize_t read_proc(struct file* filp, char __user* buf, size_t count, loff_t* offp) {
    if (*offp > 0) {
        return 0;
    }
    if (count > temp) {
        count = temp;
    }
    temp = temp - count;

    if (copy_to_user(buf, msg, count)) {
        return -EFAULT;
    }

    if (count == 0) {
        temp = len;
    }

    *offp += count;
    return count;
}

ssize_t write_proc(struct file* filp, const char __user* buf, size_t count, loff_t* offp) {
    if (count > 100) {
        count = 100;
    }

    if (copy_from_user(msg, buf, count)) {
        return -EFAULT;
    }

    len = count;
    temp = len;
    msg[count] = '\0';
    return count;
}

static const struct proc_ops proc_fops = {
    .proc_read = read_proc,
    .proc_write = write_proc,
};

void create_new_proc_entry(void) {
    proc_create(PROC_NAME, 0666, NULL, &proc_fops);
    msg = kmalloc(100 * sizeof(char), GFP_KERNEL);
    if (!msg) {
        pr_alert("Failed to allocate memory\n");
    }
}

static int __init proc_init(void) {
    create_new_proc_entry();
    pr_info("proc file created\n");
    return 0;
}

static void __exit proc_cleanup(void) {
    remove_proc_entry(PROC_NAME, NULL);
    kfree(msg);
    pr_info("proc file removed\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Fantomas");
module_init(proc_init);
module_exit(proc_cleanup);
