#include "generator_mod.h"

struct timer_list myTimer;
int keep_updating;

static int my_init(void)
{
	keep_updating = 1;
	printk(KERN_INFO "RAND_GEN->module inserted\n");
	//check non-zero return
	if(prepare_timer()){
		printk(KERN_ALERT "RAND_GEN->unable to initialize timer!");
	}
	printk(KERN_INFO "RAND_GEN->initialized timer\n");
	return 0;
}
static void my_exit(void)
{
	keep_updating = 0;
	del_timer(&myTimer);
	printk(KERN_INFO "RAND_GEN->removed module\n");
}

int prepare_timer(){
	init_timer(&myTimer);
	// call every second
	myTimer.expires = jiffies + HZ;
	myTimer.function = read_gpio;
	add_timer(&myTimer);
	return 0;
}

int update_timer(){
	return mod_timer(&myTimer, jiffies + HZ);
}

void read_gpio(unsigned long data){
	printk(KERN_INFO "Timer loop");
	if(keep_updating && !update_timer()){
		printk(KERN_ALERT "RAND_GEN->unable to update timer!");
	}
}

MODULE_LICENSE("GPL");

module_init(my_init);
module_exit(my_exit);
