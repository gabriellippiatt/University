/*
*************************************************************************
* @file oslib/scu_drivers/scu_ble/scu_ble.h
* @author Fardeen Bhuiyan - 45296635
* @date 1-04-2021
* @brief Sensor functionality
*************************************************************************
* EXTERNAL FUNCTIONS
*************************************************************************
*void ble_adv(void);
*void peph_init(void);
*void scanner_init(void);
*void bt_init(void);
*void load(double sensorData1[],int start,int len);
*************************************************************************
*/
#ifndef SCU_BLE
#define SCU_BLE
#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
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

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)
#define sensorId 7
void ble_adv(void);
void peph_init(void);
void scanner_init(void);
void bt_init(void);
void load(double sensorData1[],int start,int len);
void scan_argon_init(void(*argon_found)(const bt_addr_le_t *addr, int8_t rssi, uint8_t adv_type, struct net_buf_simple *buf));
void load2b(double sensorData1[],int start,int len);
extern bool found;
extern bool loadTemp;
extern bool loadHum;
extern bool loadAp;
extern bool loadVoc;
extern bool loadX;
extern bool loadY;
extern bool loadZ;
extern bool loadBut;
extern bool ledRGB;
extern bool loadAll;
extern bool buzz;
extern bool retDC;
extern bool retSample;

extern uint8_t buzzFH;
extern uint8_t buzzFL;
extern uint8_t sampleH;
extern uint8_t sampleL;

extern uint8_t redVal;
extern uint8_t blueVal;
extern uint8_t greenVal;
extern double dutyVal;
extern double sensorAdv[4];
extern double allAdv[18];
extern double defAd[18];
extern double accelAdv[7];
#endif