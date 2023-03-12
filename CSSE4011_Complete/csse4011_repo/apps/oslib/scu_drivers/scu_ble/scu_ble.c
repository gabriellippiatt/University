/*
*************************************************************************
* @file oslib/scu_drivers/scu_ble/scu_ble.c
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
#include "scu_ble.h"


/*
 * Set Advertisement data. Based on the Eddystone specification:
 * https://github.com/google/eddystone/blob/master/protocol-specification.md
 * https://github.com/google/eddystone/tree/master/eddystone-url
 */
 //RSSI1,RSSI2,temp,hum,x,y,z,airpressure,voc
//uint8_t data[20] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
//empty epmpty rssi1 rssi2 rssi3 rssi4 rssi5 rssi6 rssi7 rssi8 rssi9 rssi10 rssi11 rssi12,us1[0],us1[1],us2[0],us2[1],time1,time2
uint8_t data[20] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t data1[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
double sensorAdv[4] = {170,2,8,0};
double accelAdv[7] = {170,2,16,0,0,0,0};
double allAdv[18] = {0xAA,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
double defAd[18] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//struct bt_data ad[] = {BT_DATA(BT_DATA_SVC_DATA32, data,3),};
struct bt_data ad[] = {BT_DATA(BT_DATA_SVC_DATA32, data,20),};
struct bt_data ad1[] = {BT_DATA(BT_DATA_SVC_DATA32, data1,16),};
// all flags that indicate if an IO/sensor Data has been requested
bool found = false;
bool loadTemp = false;
bool loadHum = false;
bool loadAp = false;
bool loadVoc = false;
bool loadX = false;
bool loadY = false;
bool loadZ = false;
bool loadBut = false;
bool ledRGB = false;
bool buzz = false;
bool retDC = false;
bool retSample = false;
bool loadAll = false;
// variables that holds the IO values and sensor values
uint8_t redVal = 0;
uint8_t blueVal = 0;
uint8_t greenVal = 0;
uint8_t buzzFH = 0;
uint8_t buzzFL = 0;
double  dutyVal = 100;
uint8_t sampleH = 0;
uint8_t sampleL = 0;
static struct bt_le_scan_param scan_param = {
    .type       = BT_HCI_LE_SCAN_PASSIVE,
    .options    = BT_LE_SCAN_OPT_NONE,
    .interval   = 0x190,
    .window     = 0x190,
};

// scanning data
static uint8_t mfg_data[] = { 0xff, 0xff, 0x00 };

/**
function that initalises bluetooth
**/
void bt_init(void) {
	bt_enable(NULL);
}

/* Set Scan Response data */
static const struct bt_data sd[] = {
	BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
};

/**
function that advertises the requested packet
**/
void load(double sensorData1[],int start,int len) {
	int c = 0;
	for(int i = start; i < start + len; i++) {
			data[i] = (uint8_t)sensorData1[c];
			c++;
	}
	bt_le_adv_update_data(ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));

}

/**
function that advertises the requested packet
**/
void load2b(double sensorData1[],int start,int len) {
	int c = 0;
	for(int i = start; i < start + len; i++) {
			data[i] = (uint8_t)sensorData1[c];
			c++;
	}
	bt_le_adv_update_data(ad1, ARRAY_SIZE(ad1), sd, ARRAY_SIZE(sd));

}
/**
initialise bt advertising
**/
void ble_adv(void)
{
	char addr_s[BT_ADDR_LE_STR_LEN];
	bt_addr_le_t addr = {0};
	size_t count = 1;

    bt_le_adv_start(BT_LE_ADV_PARAM(BT_LE_ADV_OPT_USE_IDENTITY, \
                    BT_GAP_ADV_FAST_INT_MIN_1, \
                    BT_GAP_ADV_FAST_INT_MAX_1, \
                    NULL), ad, ARRAY_SIZE(ad),
                    sd, ARRAY_SIZE(sd));

	bt_id_get(&addr, &count);
	bt_addr_le_to_str(&addr, addr_s, sizeof(addr_s));
}


/**
*function that handles the scan call back, that finds the device and scanns packets from the dongle
*@param addr - bluetooth address
*@param rssi - strength of signal
*@param adv_type - type of advertsing
*@param - data buffer
**/
static void scan_cb(const bt_addr_le_t *addr, int8_t rssi, uint8_t adv_type,
		    struct net_buf_simple *buf)

{	
	// dongle 1 address 
	uint8_t donglead[6] = {0x92,0xCA,0x3F,0x32,0xC0,0xE3};
	// dongle 2 address
	uint8_t donglead2[6] = {0x39,0x5F,0x44,0x7E,0x1A,0xD8};
	//counter to check for connection
	int counter = 0;
	for(int i = 0; i < 6; i++) {
		if(addr->a.val[i] == donglead[i]) {
			counter++;
		}
	}
	// counter is 6 means there was a device ID match
	if(counter == 6) {
		found = true;
	}
	counter = 0;

	for(int i = 0; i < 6; i++) {
		if(addr->a.val[i] == donglead2[i]) {
		counter++;
		}
	}
	if(counter == 6) {
		found = true;
	}
	counter = 0;

	// check if any IO/sensor data was requested
	if(found) {
		if(buf -> data[sensorId] == 0x01) {
			loadTemp = true;
		}
		if(buf -> data[sensorId] == 0x02) {
			loadHum = true;
		}
		if(buf -> data[sensorId] == 0x03) {
			loadAp = true;
			
		}
		if(buf -> data[sensorId] == 0x04) {
			loadVoc = true;
			
		}
		if(buf -> data[sensorId] == 0x05) {
			loadX = true;
			
		}
		if(buf -> data[sensorId] == 0x06) {
			loadY = true;
			
		}
		if(buf -> data[sensorId] == 0x07) {
			loadZ = true;
		}
		if(buf -> data[sensorId] == 0x0A) {
			loadBut = true;
			
		}
		if(buf -> data[sensorId] == 0x08) {
			ledRGB = true;
			redVal = buf -> data[sensorId+1];
			greenVal = buf -> data[sensorId+2];
			blueVal = buf -> data[sensorId+3];
		}
		if(buf -> data[sensorId] == 0x09) {
			buzz = true;
			buzzFH = buf -> data[sensorId + 1];
			buzzFL = buf -> data[sensorId + 2];
		}
		if(buf -> data[sensorId] == 0xEE) {
			retDC = true;
			dutyVal = buf -> data[sensorId + 1];
		}
		if(buf -> data[sensorId] == 0xDD) {
			retSample = true;
			sampleH = buf -> data[sensorId + 1];
			sampleL = buf -> data[sensorId + 2];
		}
		if(buf -> data[sensorId] == 0xFF) {
			loadAll = true;
		}
		found = false;
	}
	mfg_data[2]++;
}

/**
*initalise scanning
**/
void scanner_init(void) {
	 bt_le_scan_start(&scan_param, scan_cb);
}

void scan_argon_init(void(*argon_found)(const bt_addr_le_t *addr, int8_t rssi, uint8_t adv_type, struct net_buf_simple *buf)) {
	bt_le_scan_start(&scan_param,argon_found );

}