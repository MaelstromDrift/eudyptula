#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

MODULE_AUTHOR("Mark");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("misc_eud: proj 6 module.");

static int misc_open(struct inode*, struct file*);
static int misc_release(struct inode*, struct file*);
static ssize_t misc_read(struct file*, char __user*, size_t, loff_t*);
static ssize_t misc_write(struct file*, const char __user*, size_t, loff_t*);

struct file_operations f_ops = {
    .open = &misc_open,
    .release = &misc_release,
    .read = &misc_read,
    .write = &misc_write
};
static struct miscdevice misc_dev;

static int __init mod_init(void) {
    misc_dev.minor = MISC_DYNAMIC_MINOR;
    misc_dev.name = "eud";
    misc_dev.fops = &f_ops;

    int ret = misc_register(&misc_dev);

    if(ret) {
        return ret;
    }
    
    printk("misc_eud: Mod loaded, Minor value assigned %i\n", misc_dev.minor);

    return 0;
}

static void __exit mod_exit(void) {
    misc_deregister(&misc_dev);

    printk("Mod unloaded\n");
}

static int misc_open(struct inode *i_node, struct file *filp) {
    printk("misc_eud: Open\n");
    return 0;
}

static int misc_release(struct inode *i_node, struct file *filp) {
    printk("misc_eud: Release\n");
    return 0;
}

static ssize_t misc_read(struct file *filp, char __user *buf, size_t buf_size, loff_t *pos) {
    ssize_t bytes_read = 0;
    char data[6] = "markg\0";

    if(*pos > 0) {
        return 0;
    }

    ssize_t copied = copy_to_user(buf, data, sizeof(data));
    *pos += sizeof(data) - copied;

    return sizeof(data) - copied;
}

static ssize_t misc_write(struct file *filp, const char __user *buf, size_t buf_size, loff_t *pos) {
    ssize_t bytes_written = 0;
    char data[32] = "markg";
    char user_data[32] = "";

    if(*pos > 0) {
        *pos = 0;
        return 0;
    }

    if(buf_size != 6) {
        printk("misc_eud: Incorrect buffer size: %d\n", buf_size);
        return -EINVAL;
    }
    
    ssize_t res = copy_from_user(user_data, buf, buf_size);

    if(strncmp(data, user_data, 5) != 0) {
        printk("misc_eud: %s != %s: return %d\n", data, user_data, strncmp(data, user_data, 5));
        return -EINVAL;
    }

    *pos += 6;
    return sizeof(user_data) - res;
}

module_init(mod_init);
module_exit(mod_exit);
