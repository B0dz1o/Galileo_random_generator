#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/jiffies.h>

struct timer_list myTimer;

static int my_init(void)
{
	printk(KERN_INFO "RAND_GEN->initialized\n");
	if(!prepare_timer()){
		printk(KERN_ALERT "RAND_GEN->unable to initialize timer!");
	}
	return 0;
}
static void my_exit(void)
{
	printk(KERN_INFO "RAND_GEN->removed module\n");
}

int prepare_timer(){
	static long int loopIndex = 0;
	init_timer(&myTimer);
	mt.data = ++loopIndex;
	// call every second
	myTimer.expires(jiffies + HZ);
	mt.function = read_gpio;
	add_timer(&mt);
}

void read_gpio(unsigned long data){
	printk(KERN_INFO "Timer loop%d",data);
	if(!prepare_timer()){
		printk(KERN_ALERT "RAND_GEN->unable to initialize timer!");
	}
}

MODULE_LICENSE("GPL");

module_init(my_init);
module_exit(my_exit);
