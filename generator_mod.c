/**
Linux kernel module, reading from analog inputs of Galileo board
and feeding that data to system entropy pool
author: Piotr Bogdan
*/

#include "generator_mod.h"

struct hwrng this_device;
struct iio_channel channel_analog;

static int my_init(void)
{	
	this_device.name = "A0_till_A2";
//	this_device.quality = 100;
///quality field is not present in 3.8.* kernel
	this_device.read = read_hw_random;
	hwrng_register(&this_device);
	printk(KERN_INFO "RAND_GEN->inserted module\n");
	return 0;
}
static void my_exit(void)
{	
	hwrng_unregister(&this_device);
	printk(KERN_INFO "RAND_GEN->removed module\n");
}

int read_analog(unsigned int gpio_allow, int analog_src){
	///analog ports all have alternative functions; to use them as inputs
	///certain gpio ports must be set to "output 0"
	///also, to have some visible effect, the in-board diode blink whenever this
	///function is called from hw_random driver
	const int diode_gpio = 3;
	int err_value, diode_err_value;
	char analog_value[8] = "";
	diode_err_value = gpio_request(diode_gpio,"diode");
	if (diode_err_value == 0){
		diode_err_value = gpio_direction_output(diode_gpio,0);
		if (diode_err_value == 0){
			gpio_set_value(diode_gpio,0);
		}
	}
	printk(KERN_ALERT "diode:%d\n",diode_err_value);
	err_value = gpio_request(gpio_allow,"ALLOW_ADC");
	printk(KERN_ALERT "Request:%d\n",err_value);
	if (err_value == 0){
		err_value = gpio_direction_output(gpio_allow, 0);
		printk(KERN_ALERT "Direction output:%d\n",err_value);
		if(err_value == 0){
			gpio_set_value(gpio_allow,0);	
//READING FROM ANALOG CRASHES SYSTEM
/*			
			channel_analog = *(iio_channel_get("iio:device0","A_input"));
			iio_channel_release(&channel_analog);
*/
		}
	}
	if (diode_err_value == 0){
		gpio_set_value(diode_gpio,1);
		gpio_free(diode_gpio);
	}
	gpio_free(gpio_allow);
	if (err_value != 0){
		///in case of error, send through its code
		printk(KERN_ALERT "Analog read error:%d\n",err_value);
		return err_value;
	} else	
		printk(KERN_ALERT "Read value:%s\n",analog_value);
		return 0;
}


int read_hw_random(struct hwrng *rng, void* data, size_t max, bool wait) {
	int myData[3];
	int count = 0,val;
	val = read_analog(37,0);
	if(!val){
		myData[count] = val;
		++count;
	}
	val = read_analog(36,1);
	if(!val){
		myData[count] = val;
		++count;
	}
	val = read_analog(23,2);
	if(!val){
		myData[count] = val;
		++count;
	}
	printk(KERN_INFO "data read:%d\n",count);	
	data = myData;
	return count * sizeof(int);
}

MODULE_LICENSE("GPL");

///export functions my_init and my_exit
module_init(my_init);
module_exit(my_exit);
