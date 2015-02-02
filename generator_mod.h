#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/random.h>
#include <linux/syscalls.h>
#include <linux/fcntl.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/hw_random.h>

void init_gpio(void);
void read_gpio(unsigned long data);
int read_analog(unsigned int gpio_allow, int analog_src);
void add_entropy(int entSource);
int read_random(struct hwrng *rng, void* data, size_t max, bool wait);
