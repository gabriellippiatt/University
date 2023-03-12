#include "scu_us.h"

#define TRIGGER 3
#define ECHO 4

const struct device *gpio_dev;
int ret, val;
uint32_t begin, last, diff, timeDiff;

int us_init(void) {
	gpio_dev = device_get_binding("GPIO_0");
	gpio_pin_configure(gpio_dev, TRIGGER, GPIO_OUTPUT_LOW);
	gpio_pin_configure(gpio_dev, ECHO, GPIO_INPUT);
    return 0;
}

uint16_t get_range(void) {
    gpio_pin_set(gpio_dev, TRIGGER, true);
    k_sleep(K_NSEC(10000));
    gpio_pin_set(gpio_dev, TRIGGER, false);
    while(true) {
         val = gpio_pin_get(gpio_dev, ECHO);
         if(val) {
             break;
         }
    }
    begin = k_cycle_get_32();
    while(true) {
        val = gpio_pin_get(gpio_dev, ECHO);
        last= k_cycle_get_32();
        diff = last- begin;
        if (k_cyc_to_us_ceil32(diff)/58 > 400 || !val) {
            break;
        }
    }
    timeDiff = k_cyc_to_us_ceil32(diff);
        printk("Distance: %d\n\r", timeDiff/58);
    return timeDiff/58;
}