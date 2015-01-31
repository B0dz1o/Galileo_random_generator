#include "generator_mod.h"

struct timer_list myTimer;

static int my_init(void)
{
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
	int analogValue;
	analogValue = read_analog(37,0);
	printk(KERN_INFO "analog value:%d\n",analogValue);
	update_timer();
}

int read_analog(unsigned int gpio_allow, int analog_src){
	char analog_value[8];
	int errValue;
	errValue = gpio_request(gpio_allow,"AD_ENABLE");
	if (errValue == 0){
		errValue = gpio_direction_output(gpio_allow,1);
		if(errValue == 0){
			struct file *fd;
			char path[64];
			sprintf(path,"/sys/bus/iio/devices/iio\\:device0/in_voltage%d_raw",analog_src);
			fd = filp_open(path,O_RDONLY,0);
			errValue = (int) fd;
			if(fd == 0){
				fd->f_op->read(fd,analog_value,5,0);
				filp_close(fd,NULL);
			}
			gpio_free(gpio_allow);
		}
	}
	if (errValue != 0){
		printk(KERN_ALERT "Analog read error:%d\n",errValue);
		return errValue;
	} else{
		int retValue;
		char** endPoint = NULL;
		const int base = 10;
		retValue = (int) simple_strtol (analog_value,endPoint,base);
		return(retValue);
	}
}

void add_entropy(int entSource){
	struct file *fd;
	fd = filp_open("/dev/random",O_WRONLY,0);
	fd->f_op->unlocked_ioctl(fd,RNDADDENTROPY,entSource);
	filp_close(fd,NULL);
	return;
}

MODULE_LICENSE("GPL");

module_init(my_init);
module_exit(my_exit);

