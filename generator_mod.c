#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>

static int my_init(void)
{
	printk(KERN_ALERT "RAND_GEN->initialized\n");
	return 0;
}
static void my_exit(void)
{
	printk(KERN_ALERT "RAND_GEN->removed module\n");
}

MODULE_LICENSE("GPL");

module_init(my_init);
module_exit(my_exit);
