#include "generator_mod.h"

struct timer_list myTimer;

static int my_init(void)
{
	printk(KERN_INFO "RAND_GEN->module inserted\n");
	//check non-zero return
	if(prepare_timer()){
		printk(KERN_ALERT "RAND_GEN->unable to initialize timer!\n");
	}
	printk(KERN_INFO "RAND_GEN->initialized timer\n");
	return 0;
}
static void my_exit(void)
{
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
	int fd = sys_open("/dev/random",O_WRONLY,0);
	if(fd > 0){
		printk(KERN_INFO "File opened\n");
	}
	if(IS_ERR(fd)) {
		int err = PTR_ERR(fd);
		printk(KERN_ALERT "Error:%d\n",err);
	}
	sys_close(fd);
	printk(KERN_INFO "Timer loop\n");
}

MODULE_LICENSE("GPL");

module_init(my_init);
module_exit(my_exit);
