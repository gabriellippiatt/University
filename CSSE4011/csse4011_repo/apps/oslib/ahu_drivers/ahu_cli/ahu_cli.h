/*
*************************************************************************
* @file oslib/ahu_drivers/ahu_cli/ahu_cli.c
* @author Gabriel Lippiatt - 45296710
* @date 1-04-2021
* @brief cli functionality
*************************************************************************
* EXTERNAL FUNCTIONS
*************************************************************************
*void cli_init(void)
*************************************************************************
*/

#ifndef AHU_CLI
#define AHU_CLI

#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <usb/usb_device.h>
#include <drivers/uart.h>
#include <shell/shell.h>
#include <logging/log.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define waitTime 2000

void cli_init(void);
extern bool vocCli;
extern bool tempCli;
extern bool humCli;
extern bool apCli;
extern bool xCli;
extern bool yCli;
extern bool zCli;
extern bool ledCom;
extern bool redOn;
extern bool blueOn;
extern bool greenOn;
extern bool buttonCli;
extern bool allOn;
extern bool allOff;
extern bool buzz;
extern bool dc;
extern bool sample;

extern uint8_t temp;
extern uint8_t humidity;
extern uint8_t voc;
extern uint8_t airpressure;
extern float xaccel;
extern float yaccel;
extern float zaccel;
extern uint8_t button;
extern uint16_t sampleTime;
extern uint16_t buzzerFreq;
extern uint8_t dcVal;
#endif