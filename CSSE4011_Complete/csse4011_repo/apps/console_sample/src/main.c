/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <drivers/gpio.h>
#include <usb/usb_device.h>
#include <drivers/uart.h> 
#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <drivers/i2c.h>
#include <string.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)
#define I2C DT_ALIAS(i2c1)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

// #ifdef _DEBUG_ULTRASONIC
//     #include <sys/printk.h>
// #endif

// HC SR04
// Connect Trig to A0
// Echo to A1

#define HCSR04_TRIG_PIN 33
#define HCSR04_ECHO_PIN 34

const struct device *gpio_dev;
int ret, val;
uint32_t startCycle, stopCycle, deltaCycle, deltaTimeUS;

int s_ultrasonic_init(void) {
	gpio_dev = device_get_binding("GPIOD");
	if (!gpio_dev) {
       // #ifdef _DEBUG_ULTRASONIC
		printk("Cannot init gpio!\n");
       // #endif
        return -1;
	}

	ret = gpio_pin_configure(gpio_dev, HCSR04_TRIG_PIN, GPIO_OUTPUT_LOW);
    if (ret) {
        //#ifdef _DEBUG_ULTRASONIC
		printk("Error configuring TRIG\n\r");
        //#endif
        return -2;
	}

	ret = gpio_pin_configure(gpio_dev, HCSR04_ECHO_PIN, GPIO_INPUT);
    if (ret) {
       // #ifdef _DEBUG_ULTRASONIC
		printk("Error configuring ECHO\n\r");
       // #endif
        return -3;
	}
    return 0;
}

uint16_t s_ultrasonic_get_range(void) {
    // Send initial 10us pulse
    gpio_pin_set(gpio_dev, HCSR04_TRIG_PIN, true);
    //delay 10us
    k_sleep(K_NSEC(10000));
    gpio_pin_set(gpio_dev, HCSR04_TRIG_PIN, false);

    // Read ECHO pin pulse
    do {
        val = gpio_pin_get(gpio_dev, HCSR04_ECHO_PIN);
    } while (val == 0);
    startCycle = k_cycle_get_32();

    // Time the Echo pin pulse
    do {
        val = gpio_pin_get(gpio_dev, HCSR04_ECHO_PIN);
        stopCycle = k_cycle_get_32();
        deltaCycle = stopCycle - startCycle;
        if (k_cyc_to_us_ceil32(deltaCycle)/58 > 450) {
            break;
        }
    } while (val == 1);
    
    // Convert clock cycles to time
    deltaTimeUS = k_cyc_to_us_ceil32(deltaCycle);
    //#ifdef _DEBUG_ULTRASONIC
        printk("Distance: %d\n\r", deltaTimeUS/58);
    //#endif
    return deltaTimeUS/58;
}



void ter(void) {
    printk("yea yea yea\n");

}

void main(void)
{
	// int ret;

	// if (!device_is_ready(led.port)) {
	// 	return;
	// }

	// ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	// if (ret < 0) {
	// 	return;
	// }

	// while (1) {
	// 	ret = gpio_pin_toggle_dt(&led);
	// 	if (ret < 0) {
	// 		return;
	// 	}
	// 	k_msleep(SLEEP_TIME_MS);
	// }
	 /* Setup DTR */
    const struct device *console_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_console));
    uint32_t dtr = 0;

    /* Enable the USB Driver */
    if (usb_enable(NULL))   
        return;

   /* Wait on DTR - 'Data Terminal Ready'
    * Will wait here until a terminal has been attached to the device
    * This is not necessary, however, can be useful for printing boot info etc..
    */
    while (!dtr) {
        uart_line_ctrl_get(console_dev, UART_LINE_CTRL_DTR, &dtr);
        k_sleep(K_MSEC(100));
    }
    ter();
    s_ultrasonic_init();
    while(1)
    {
       // printk("Hello World\n");
        s_ultrasonic_get_range();
        k_sleep(K_MSEC(500));
    }
}
