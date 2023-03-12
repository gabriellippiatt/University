/*
*************************************************************************
* @file oslib/ahu_drivers/ahu_button/ahu_button.h
* @author Gabriel Lippiatt - 45296710
* @date 1-04-2021
* @brief button functionality
*************************************************************************
* EXTERNAL FUNCTIONS
*************************************************************************
*void button_init(void);
*************************************************************************
*/
#ifndef AHU_BUTTON
#define AHU_BUTTON
#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include <sys/util.h>
#include <sys/printk.h>
#include <inttypes.h>
#include <logging/log.h>

#define SLEEP_TIME_MS	1
extern bool pressed;
void button_init(void);
#endif