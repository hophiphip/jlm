#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cred.h>
#include <linux/version.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Author?");
MODULE_DESCRIPTION("Just a Linux Module");
MODULE_VERSION("0.0.1");

#define DEVICE_NAME    "jlm"
#define DEVICE_MSG     "jlm device\n"
#define DEVICE_BUF_LEN 1024

static int major_number;
static int device_open_count = 0;

static char msg_buf[DEVICE_BUF_LEN];
static char *msg_ptr;

static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations file_ops = {
  .read    = device_read,
  .write   = device_write,
  .open    = device_open,
  .release = device_release
};

static inline bool is_root(void) {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 5, 0)
#include "linux/uidgid.h"
    return uid_eq(current_uid(), GLOBAL_ROOT_UID);
#else
    return 0 == current_uid();
#endif
}

static ssize_t device_read(struct file *f, char *buf, size_t len, loff_t *offset)
{
  int bytes_read = 0;
 
  // If `root` return nothing
  if (is_root())
    return bytes_read;

  // Check for the end of the buffer
  // and reset it to the beginning
  if (*msg_ptr == 0) {
    msg_ptr = msg_buf;
  }

  // Populate the buffer
  while (len && *msg_ptr) {
    // Access buf that is in user data (not in kernel data)
    // via `put_user`
    put_user(*(msg_ptr++), buf++);
    --len;
    ++bytes_read;
  }

  return bytes_read;
}

static ssize_t device_write(struct file *f, const char *buf, size_t len, loff_t *offset)
{
  // NOTE: Device is read-only for now
  printk(KERN_ALERT "`Write` operation is not supported\n");
  return -EINVAL;
}

static int device_open(struct inode *inode, struct file *f)
{
  // Check if device is already open
  if (device_open_count) {
    return -EBUSY;
  }

  ++device_open_count;
  try_module_get(THIS_MODULE);

  return 0;
}

static int device_release(struct inode *inode, struct file *f)
{
  // Decrement usage count
  --device_open_count;
  module_put(THIS_MODULE);
  return 0;
}

static int  __init jlm_init(void) 
{
  // Initialize the buffer
  strncpy(msg_buf, DEVICE_MSG, DEVICE_BUF_LEN);
  msg_buf[strlen(DEVICE_MSG) + 1] = 0;
  // Set initial message pointer
  msg_ptr = msg_buf;
  
  major_number = register_chrdev(0, DEVICE_NAME, &file_ops);
  if (major_number < 0) {
    printk(KERN_ALERT "Failed to register the character device: %d\n", major_number);
    return major_number;
  }
  else {
    printk(KERN_INFO "Registered device with major number: %d\n", major_number);
  }

  printk(KERN_INFO "JLM Started\n");
  return 0;
}

static void __exit jlm_exit(void)
{
  unregister_chrdev(major_number, DEVICE_NAME);

  printk(KERN_INFO "JLM Exited\n");
}

module_init(jlm_init);
module_exit(jlm_exit);
