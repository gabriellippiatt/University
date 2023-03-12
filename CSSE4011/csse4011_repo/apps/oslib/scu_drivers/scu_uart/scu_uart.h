#ifndef SCU_UART
#define SCU_UART
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

void uart_init(void);
#endif