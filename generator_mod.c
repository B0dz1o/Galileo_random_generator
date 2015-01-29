#include "generator_mod.h"

struct timer_list myTimer;

static int my_init(void)
{
	printk(KERN_INFO "RAND_GEN->module inserted\n");
	//check non-zero return
	if(prepare_timer()){
		printk(KERN_ALERT "RAND_GEN->unable to initialize timer!\n");
	}
	init_gpio();
	printk(KERN_INFO "RAND_GEN->initialized timer\n");
	return 0;
}
static void my_exit(void)
{
	del_timer(&myTimer);
	printk(KERN_INFO "RAND_GEN->removed module\n");
	gpio_free(19);
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

void init_gpio(){
	struct file *fd;
}

void read_gpio(unsigned long data){
	static int onOff = 0;
	int xJoy,yJoy,iR;
	struct file *fd;
	char label[32];
	int result = gpio_request(19,"PINSET");
	fd = filp_open("/sys/bus/iio/devices/iio\:device0/in_voltage0_raw",O_RDONLY,0);
//	fd->f_op->read(fd,label,5,0);	
	filp_close(fd,NULL);

	printk(KERN_INFO "val:%s\n",label);
	add_entropy(1,2,3);
	update_timer();
}

void add_entropy(int xJoy, int yJoy, int iR){
	struct file *fd;
	fd = filp_open("/dev/random",O_WRONLY,0);
	fd->f_op->unlocked_ioctl(fd,RNDADDENTROPY,xJoy);
	fd->f_op->unlocked_ioctl(fd,RNDADDENTROPY,yJoy);
	fd->f_op->unlocked_ioctl(fd,RNDADDENTROPY,iR);
	filp_close(fd,NULL);
	return;
}

MODULE_LICENSE("GPL");

module_init(my_init);
module_exit(my_exit);
