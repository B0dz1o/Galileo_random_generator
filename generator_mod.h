#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/jiffies.h>

int prepare_timer(void);
void read_gpio(unsigned long data);
