/**
Linux kernel module, reading from analog inputs of Galileo board
and feeding that data to system entropy pool
author: Piotr Bogdan
*/

#include "generator_mod.h"

struct timer_list myTimer;

static int my_init(void)
{
	///check non-zero return
	if(prepare_timer()){
		printk(KERN_ALERT "RAND_GEN->unable to initialize timer!\n");
	} else {
		printk(KERN_INFO "RAND_GEN->initialized timer\n");
	}
	return 0;
}
static void my_exit(void)
{
	///remove timer to break from loop
	del_timer(&myTimer);
	printk(KERN_INFO "RAND_GEN->removed module\n");
}

int prepare_timer(){
	///create timer that allows kernel module to work continuously
	init_timer(&myTimer);
	///call every second
	myTimer.expires = jiffies + HZ;
	myTimer.function = read_gpio;
	add_timer(&myTimer);
	return 0;
}

int update_timer(){
	///update timer - do the same in 1 second
	return mod_timer(&myTimer, jiffies + HZ);
}

void read_gpio(unsigned long data){
	///check 3 analog inputs and read data from them in case of non-error return
	int analogValue;
	analogValue = read_analog(37,0); ///read from A0
	if (analogValue >= 0) {
		add_entropy(analogValue);
	}
	analogValue = read_analog(36,1); ///read from A1
	if (analogValue >= 0) {
		add_entropy(analogValue);
	}
	analogValue = read_analog(23,2); ///read from A2
	if (analogValue >= 0) {
		add_entropy(analogValue);
	}
	///continue working, repeat every second
	update_timer();
}

int read_analog(unsigned int gpio_allow, int analog_src){
	///analog ports all have alternative functions; to use them as inputs
	///certain gpio ports must be set to "output 0"
	char analog_value[8];
	int errValue;
	errValue = gpio_request(gpio_allow,"AD_ENABLE");
	if (errValue == 0){
		errValue = gpio_direction_output(gpio_allow,0);
		///set 0 output so that analog port can be used as input
		if(errValue == 0){
			struct file *fd;
			char path[64];
			///prepare path to read from chosen analog input, A0-A2
			sprintf(path,"/sys/bus/iio/devices/iio\\:device0/in_voltage%d_raw",analog_src);
			fd = filp_open(path,O_RDONLY,0);
			errValue = (int) fd;
			if(fd == 0){
				///if successfully opened, read data and close descriptor
				fd->f_op->read(fd,analog_value,5,0);
				filp_close(fd,NULL);
			}
			///stop using gpio port
			gpio_free(gpio_allow);
		}
	}
	if (errValue != 0){
		///in case of error, send through its code
		printk(KERN_ALERT "Analog read error:%d\n",errValue);
		return errValue;
	} else{
		///if no error, convert char* analog output to int, which can be passed
		///to entropy pool
		int retValue;
		char** endPoint; ///simple_strtol returns pointer to the last digit read
		const int base = 10; ///numeric system base
		retValue = (int) simple_strtol (analog_value,endPoint,base);
		return retValue;
	}
}

void add_entropy(int entSource){
	///add new data to system entropy pool, using predefined ioctl interface
	struct file *fd;
	fd = filp_open("/dev/random",O_WRONLY,0);
	if(fd == 0){
		///successful file descriptor open
		fd->f_op->unlocked_ioctl(fd,RNDADDENTROPY,entSource);
		filp_close(fd,NULL);
	}
	return;
}

MODULE_LICENSE("GPL");

///export functions my_init and my_exit
module_init(my_init);
module_exit(my_exit);
