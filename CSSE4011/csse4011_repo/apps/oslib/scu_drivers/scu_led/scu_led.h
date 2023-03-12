/*
*************************************************************************
* @file oslib/scu_drivers/scu_led/scu_led.h
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
#ifndef SCU_LED
#define SCU_LED
#include <zephyr.h>
#include <drivers/gpio.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME  1000

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)


void led_init(void);
void led_red(int i);
void led_green(int i);
void led_blue(int i);

#endif