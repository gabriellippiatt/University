/*
*************************************************************************
* @file oslib/scu_drivers/scu_sesnors/scu_sensors.h
* @author Fardeen Bhuiyan - 45296635
* @date 1-04-2021
* @brief Sensor functionality
*************************************************************************
* EXTERNAL FUNCTIONS
*************************************************************************
* void htts221_read(void);
* void lis2dh_read(void);
* void lps22hb_read(void);
* void VOC_READ(void);
*************************************************************************
*/

#ifndef SCU_SENSORS
#define SCU_SENSORS
#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/util.h>
#include <sys/printk.h>
#include <drivers/sensor/ccs811.h>

extern double sensorData[7];
void htts221_read(void);
void lis2dh_read(void);
void lps22hb_read(void);
void VOC_READ(void);
#endif