#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>

static int my_init(void)
{
	printk(KERN_ALERT "Modul z bledem 1\nZaczynam inicjalizacje\n");	
	printk(KERN_ALERT "Inicjalizacja zakonczona\n");
	return 0;
}
static void my_exit(void)
{
	printk(KERN_ALERT "Do widzenia\n");
}

MODULE_LICENSE("GPL");

module_init(my_init);
module_exit(my_exit);


