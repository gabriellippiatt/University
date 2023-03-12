/*
*************************************************************************
* @file oslib/ahu_drivers/ahu_ble/ahu_ble.h
* @author Gabriel Lippiatt - 45296710
* @date 1-04-2021
* @brief blue tooth functionality
*************************************************************************
* EXTERNAL FUNCTIONS
*************************************************************************
*void bt_init(void);
*void ble_ahu_adv(void);
*void scan_init(void);
*void load(uint8_t sensorData1[],int start, int len)
*************************************************************************
*/
#ifndef AHU_BLE
#define AHU_BLE
#include <zephyr/types.h>
#include <stddef.h>
#include <errno.h>
#include <sys/printk.h>
#include <sys/byteorder.h>
#include <zephyr.h>
#include <settings/settings.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>
#include <bluetooth/services/bas.h>
#include <bluetooth/services/hrs.h>
#include <drivers/gpio.h>
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

// set device names and len
#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

// index which sensor ID can be found from packet and sensor value packet begins
#define sensorId 5
#define sensorVal 7

//refer to ahu_ble.c for purpose of these varibles
extern char addr_str[BT_ADDR_LE_STR_LEN];
extern char check[2];
extern uint8_t requestTempSensor[4];
extern uint8_t requestHumSensor[4];
extern uint8_t requestAirPressure[4];
extern uint8_t requestVOC[4];
extern uint8_t defAddy[7];
extern uint8_t requestX[4];
extern uint8_t requestY[4];
extern uint8_t requestZ[4];
extern uint8_t requestBut[4];
extern uint8_t requestLed[7];
extern uint8_t requestBuzz[6];
extern uint8_t requestDC[5];
extern uint8_t requestSample[6];
extern uint8_t requestAll[4];
extern double allData[7][100];

// variables that store all values
extern uint8_t tempVal;
extern uint8_t humVal;
extern uint8_t apVal;
extern uint8_t vocVal;
extern double xVal;
extern double yVal;
extern double zVal;
extern uint8_t butVal;
extern uint32_t time;

// boolean flags to check if an ack response has been recieved
extern bool tempRequest;
extern bool humRequest; 
extern bool apRequest;
extern bool vocRequest;
extern bool xRequest;
extern bool yRequest;
extern bool zRequest;
extern bool butRequest;
extern bool ledRequest;
extern bool red;
extern bool blue;
extern bool green;
extern bool allRequest;
extern bool xNeg;
extern bool sampleT;
extern bool buzzReq;
extern bool dcReq;
extern bool sampleReq;
extern bool sampleOn;

void bt_init(void);
void ble_ahu_adv(void);
void scan_init(void);
void load(uint8_t sensorData1[],int start, int len);
void scan_mobile_init(void(*mobile_found)(const bt_addr_le_t *addr, int8_t rssi, uint8_t adv_type, struct net_buf_simple *buf));
#endif