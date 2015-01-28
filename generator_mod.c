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
	struct file *fd = filp_open("/dev/random",O_RDONLY,0);
//	int fd = filp_open("/sys/class/gpio/export",O_WRONLY,0);
	if(fd == -1){
		printk(KERN_ALERT "Unable to open file\n");
	}

	fd->f_op->unlocked_ioctl(fd,RNDADDENTROPY,170);
	filp_close(fd,NULL);
	printk(KERN_INFO "Timer loop\n");
	update_timer();
}

MODULE_LICENSE("GPL");

module_init(my_init);
module_exit(my_exit);
