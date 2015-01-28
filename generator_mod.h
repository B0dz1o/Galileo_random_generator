#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/random.h>

int prepare_timer(void);
int update_timer(void);
void read_gpio(unsigned long data);
