#ifndef SCU_US
#define SCU_US
#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <drivers/i2c.h>
#include <string.h>
#include <sys/printk.h>
int us_init(void);
uint16_t get_range(void);
#endif