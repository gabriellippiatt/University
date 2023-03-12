/*
*************************************************************************
* @file oslib/scu_drivers/scu_led/scu_led.c
* @author Fardeen Bhuiyan - 45296635
* @date 1-04-2021
* @brief Sensor functionality
*************************************************************************
* EXTERNAL FUNCTIONS
*************************************************************************
*void led_init(void);
*void led_red(int i);
*void led_green(int i);
*void led_blue(int i);
*************************************************************************
*/
#include "scu_led.h"
// variables that controls the led variable
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(LED2_NODE, gpios);

/**
*function that sets or resets the red led
**/
void led_red(int i ) {
    gpio_pin_set_dt(&led,i);
}

/**
*function that sets or resets the green led
**/
void led_green(int i) {
    gpio_pin_set_dt(&led1,i);
}

/**
*function that sets or resets the blue led
**/
void led_blue(int i) {
    gpio_pin_set_dt(&led2,i);
}

/**
*function which initalises the led ports
**/
void led_init(void) {
	int ret;

	if (!device_is_ready(led.port)) {
		return;
	}
    if (!device_is_ready(led1.port)) {
	    return;
	}
    if (!device_is_ready(led2.port)) {
		return;
	}
	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    ret = gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE);
    ret = gpio_pin_configure_dt(&led2, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return;
	}
    gpio_pin_set_dt(&led,0);
    gpio_pin_set_dt(&led1,0);
    gpio_pin_set_dt(&led2,0);
}