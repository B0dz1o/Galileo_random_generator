#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/random.h>
#include <linux/syscalls.h>
#include <linux/fcntl.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

int prepare_timer(void);
int update_timer(void);
void init_gpio(void);
void read_gpio(unsigned long data);
void add_entropy(int joyX,int joyY,int iR);
