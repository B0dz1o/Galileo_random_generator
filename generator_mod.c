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
	char buf[] = "37";
	fd = filp_open("/sys/class/gpio/export",O_WRONLY,FMODE_WRITE);
	fd->f_op->write(fd,buf,strlen(buf),0);
	filp_close(fd,NULL);
	printk(KERN_ALERT "Should be added\n");
	add_entropy(1,2,3);
	fd = filp_open("/sys/class/gpio/export",O_WRONLY,0);
	fd->f_op->write(fd,"36",3,0);
	filp_close(fd,NULL);
	fd = filp_open("/sys/class/gpio/export",O_WRONLY,0);
	fd->f_op->write(fd,"23",3,0);
	filp_close(fd,NULL);
	// fd = filp_open("/sys/class/gpio/gpio37/direction",O_WRONLY,0);
	// fd->f_op->write(fd,"low",4,0);
	// filp_close(fd,NULL);
	// fd = filp_open("/sys/class/gpio/gpio36/direction",O_WRONLY,0);
	// fd->f_op->write(fd,"low",4,0);
	// filp_close(fd,NULL);
	// fd = filp_open("/sys/class/gpio/gpio23/direction",O_WRONLY,0);
	// fd->f_op->write(fd,"low",4,0);
	// filp_close(fd,NULL);
}

void read_gpio(unsigned long data){
	static int onOff = 0;
	int xJoy,yJoy,iR;
	struct file *fd;
//	printk(KERN_INFO "Timer loop\n");

	// fd = filp_open("/sys/class/gpio/gpio28/value",O_WRONLY,0);
	// if(onOff){
	// 	fd->f_op->write(fd,"0",1,0);
	// } else{
	// 	fd->f_op->write(fd,"1",1,0);
	// }
	// onOff = !onOff;
	// filp_close(fd,NULL);


//	add_entropy(xJoy,yJoy,iR);
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
