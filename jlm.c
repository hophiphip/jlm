#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_DESCRIPTION("Just a Linux Module");
MODULE_VERSION("0.0.1");

static int  __init jlm_init(void) {
  printk(KERN_INFO "JLM Started\n");
  return 0;
}

static void __exit jlm_exit(void) {
  printk(KERN_INFO "JML Exited\n");
}

module_init(jlm_init);
module_exit(jlm_exit);
