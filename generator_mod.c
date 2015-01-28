#include "generator_mod.h"

struct timer_list myTimer;

static int my_init(void)
{
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
	printk(KERN_INFO "RAND_GEN->removed module\n");
}

int prepare_timer(){
	static long int loopIndex = 0;
	init_timer(&myTimer);
	myTimer.data = ++loopIndex;
	// call every second
	myTimer.expires = jiffies + HZ;
	myTimer.function = read_gpio;
	add_timer(&myTimer);
	return 0;
}

void read_gpio(unsigned long data){
	printk(KERN_INFO "Timer loop%ld",data);
	if(!prepare_timer()){
		printk(KERN_ALERT "RAND_GEN->unable to initialize timer!");
	}
}

MODULE_LICENSE("GPL");

module_init(my_init);
module_exit(my_exit);
