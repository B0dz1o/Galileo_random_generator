/**
Linux kernel module, reading from analog inputs of Galileo board
and feeding that data to system entropy pool
author: Piotr Bogdan
*/

#include "generator_mod.h"

struct timer_list myTimer;

static int my_init(void)
{
	int gpio_allow = 37,errValue;
	printk(KERN_ALERT "RAND_GEN->initialized timer\n");
	errValue = gpio_request_one(gpio_allow,GPIOF_INIT_LOW,"ALLOW_ADC");
	printk(KERN_ALERT "%d gpio returned %d\n",gpio_allow,errValue);
	gpio_free(gpio_allow);

	gpio_allow = 26;
	errValue = gpio_request(gpio_allow,"ALLOW_ADC");	
	printk(KERN_ALERT "%d gpio returned %d\n",gpio_allow,errValue);
	errValue = gpio_direction_output(26,1);
	gpio_set_value(gpio_allow, 1);
	gpio_free(26);	
	printk(KERN_INFO "turned output high, code:%d\n",errValue);
	return 0;
}
static void my_exit(void)
{
	int gpio_allow = 26,errValue;
	gpio_free(26);
	errValue = gpio_request(gpio_allow,"ALLOW_ADC");	
	printk(KERN_ALERT "%d gpio returned %d\n",gpio_allow,errValue);
	errValue = gpio_direction_output(26,0);
	gpio_set_value(gpio_allow, 0);
	printk(KERN_INFO "turned output high, code:%d\n",errValue);
	gpio_free(26);	

	///remove timer to break from loop
	del_timer(&myTimer);
//	gpio_free(26);
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
	add_entropy(17);
	analogValue = read_analog(36,1); ///read from A1
	analogValue = read_analog(23,2); ///read from A1
/*	if (analogValue >= 0) {
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
*/
	///continue working, repeat every second
	update_timer();
}

int read_analog(unsigned int gpio_allow, int analog_src){
	///analog ports all have alternative functions; to use them as inputs
	///certain gpio ports must be set to "output 0"
	char analog_value[8];
	int errValue;
	struct file *fd;
	char path[64];
	errValue = gpio_request_one(gpio_allow,GPIOF_INIT_LOW,"ALLOW_ADC");
	printk(KERN_INFO "%d requested : %d\n",gpio_allow,errValue);
	if (errValue == 0){
//		errValue = gpio_direction_output(gpio_allow, 0);
		if(errValue == 0){
			printk(KERN_INFO "%d requested : %d\n",gpio_allow,errValue);
		}
	}
	gpio_free(gpio_allow);
	///prepare path to read from chosen analog input, A0-A2
	if (errValue != 0){
		///in case of error, send through its code
		printk(KERN_ALERT "Analog read error:%d\n",errValue);
		return errValue;
	} else{
		///if no error, convert char* analog output to int, which can be passed
		///to entropy pool
		int retValue;
		char** endPoint = NULL; ///simple_strtol returns pointer to the last digit read
		const int base = 10; ///numeric system base
		retValue = (int) simple_strtol (analog_value,endPoint,base);
		return retValue;
	}
}

void add_entropy(int entSource){
	///add new data to system entropy pool, using predefined ioctl interface
	struct file *fd;
	fd = filp_open("/dev/random",O_WRONLY,0);
	if(fd >= 0){
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
