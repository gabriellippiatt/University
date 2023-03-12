/*
*************************************************************************
* @file oslib/scu_drivers/scu_button/scu_button.h
* @author Fardeen Bhuiyan - 45296635
* @date 1-04-2021
* @brief Sensor functionality
*************************************************************************
* EXTERNAL FUNCTIONS
*************************************************************************
*void button_init(void)
*************************************************************************
*/
#ifndef SCU_BUTTON
#define SCU_BUTTON
#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include <sys/util.h>
#include <sys/printk.h>
#include <inttypes.h>

#define SLEEP_TIME_MS	1
extern bool pressed;
void button_init(void);
#endif