/*
*************************************************************************
* @file oslib/ahu_drivers/ahu_ble/ahu_ble.c
* @author Gabriel Lippiatt - 45296710
* @date 1-04-2021
* @brief Sensor functionality
*************************************************************************
* EXTERNAL FUNCTIONS
*************************************************************************
*void bt_init(void);
*void ble_ahu_adv(void);
*void scan_init(void);
*void load(uint8_t sensorData1[],int start, int len)
*************************************************************************
*/

#include "ahu_ble.h"

#define LED0_NODE DT_ALIAS(led0)

int ret;
bool thingyFound = false;

/* all the sensor values initialised */
uint8_t tempVal = 0x00;
uint8_t humVal = 0x00;
uint8_t apVal = 0x00;
uint8_t vocVal = 0x00;
uint8_t butVal = 0;
double xVal = 0;
double yVal = 0;
double zVal = 0;
uint32_t time = 0;

/* arrays to store accelerometer reading packets */
uint8_t xvals[4] = {0x00,0x00,0x00,0x00};
uint8_t yvals[4] = {0x00,0x00,0x00,0x00};
uint8_t zvals[4] = {0x00,0x00,0x00,0x00};
char xString[10] = {"0x00000000"};

/* struct that handles the bt scanning */
struct bt_le_scan_param scan_param = {
	.type       = BT_HCI_LE_SCAN_PASSIVE,
	.options    = BT_LE_SCAN_OPT_NONE,
	.interval   = 0x0010,
	.window     = 0x0010,
	};

/* primary data array which will be advertised */
uint8_t data[9] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

/* packets that are advertised to retrieve sensor values */
uint8_t requestTempSensor[4] = {0xAA,0x01,0x04,0x01};
uint8_t requestHumSensor[4] = {0xAA,0x01,0x04,0x02};
uint8_t requestAirPressure[4] = {0xAA,0x01, 0x04, 0x03};
uint8_t requestVOC[4] = {0xAA,0x01,0x04,0x04};
uint8_t requestX[4] = {0xAA,0x01,0x16,0x05};
uint8_t requestY[4] = {0xAA,0x01,0x16,0x06};
uint8_t requestZ[4] = {0xAA,0x01,0x16,0x07};
uint8_t requestBut[4] = {0xAA,0x01,0x16,0x0A};
uint8_t requestLed[7] = {0xAA, 0x01, 0x16, 0x08, 0x00, 0x00, 0x00};
uint8_t requestBuzz[6] = {0xAA, 0x01, 0x04, 0x09, 0x00, 0x00};
uint8_t requestDC[5] = {0xAA,0x01,0x04,0xEE,0x00};
uint8_t requestSample[6] = {0xAA,0x01,0x04,0xDD,0x00, 0x00};
uint8_t requestAll[4] = {0xAA,0x01, 0x04,0xFF};
double allData[7][100];

/* default packet to advertise */
uint8_t defAddy[7] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/* boolean flages that indicate which sensor value has been requested by cli command */
bool tempRequest = false;
bool humRequest = false;
bool apRequest = false;
bool vocRequest = false;
bool xRequest = false;
bool yRequest = false;
bool zRequest = false;
bool butRequest = false;
bool red = false;
bool blue = false;
bool green = false;
bool ledRequest = false;
bool allRequest = false;
bool sampleT = false;
bool buzzReq = false;
bool dcReq = false;
bool sampleReq = false;
int sampleCount = 0;
bool sampleOn = false;
bool xNeg = false;

/* advertisement array */
struct bt_data ad[] = {BT_DATA(BT_DATA_SVC_DATA16, data, 9),};

/* checks if the thingy 52 has been found */
bool found = false;

/* Set Scan Response data */
static const struct bt_data sd[] = {
	BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
};

/* scan data */
static uint8_t mfg_data[] = { 0xff, 0xff, 0x00 };

/**
 * @brief converts hex to a string
 * 
 * @param hex the hex value
 * @param len number of hex values
 * @return char* 
 */
char* hex2str(unsigned char *hex,unsigned int len){
    int i;
    char *val;
    char *final;
    val = malloc(sizeof(char));
    final = malloc(strlen(hex));
    strcpy(final ,"");
    strcat(final ,"0x");
    for (i=0;i<len;i++){
    	sprintf(val,"%02x",hex[i]);
    	strcat(final,val);
    }
    return final;
}

/**
 * @brief  convert hex values to float
 * 
 * @param vals hex value split up into an array
 * @return float version of hex
**/
float hextofloat(char vals[]) {
    union {
        char c[4];
        float f;
    } u;
	for(int i = 0; i < 4; i++) {
		u.c[i] = vals[i];
	}
	return u.f;
}

/**
 * @brief function which is called when a device is scanned
 * 
 * @param addr address of device
 * @param rssi strenght of connection
 * @param adv_type type of advertisement
 * @param buf variable which stores the advertised data
 */
static void scan_cb(const bt_addr_le_t *addr, int8_t rssi, uint8_t adv_type,
		    struct net_buf_simple *buf)

{	// counter stores the amount of values which match the desired address
	int counter = 0;

	//desired thingy address
	uint8_t thingyad[6] = {0xEF,0x92,0x55,0xBB, 0x95, 0xFE};
	uint8_t thingyad2[6] = {0x9A,0x7A,0xC2,0x07, 0x7F, 0xE5};

	// loops through device adress and compares it to thingy address
	for(int i = 0; i < 6; i++) {
		if(addr->a.val[i] == thingyad[i]) {
			counter++;
		}
	}

	// if all values in current addres match, thingy has been found
	if(counter == 6) {
		thingyFound = true;
	}

	//reset the counter
	counter = 0;

	// loops through device adress and compares it to thingy address
	for(int i = 0; i < 6; i++) {
		if(addr->a.val[i] == thingyad2[i]) {
			counter++;
		}
	}

	// if all values in current addres match, thingy has been found
	if(counter == 6) {
		thingyFound = true;
	}
	counter = 0;

	// if thingy was found and check requests
	if (thingyFound) {
		if(tempRequest) {
			if(buf->data[sensorId] == 0x02) {
				tempVal = buf->data[sensorVal];
				tempRequest = false;
			}
		}
		if(humRequest) {
			if(buf->data[sensorId] == 0x02) {
				humVal = buf->data[sensorVal];
				humRequest = false;
			}
		}
		if(apRequest) {
			if(buf->data[sensorId] == 0x02) {
				 apVal = buf->data[sensorVal];
				 apRequest = false;
			}
		 }
		if(vocRequest) {
			if(buf->data[sensorId] == 0x02) {
				vocVal = buf->data[sensorVal];
				vocRequest = false;
			}
		}
		if(xRequest) {
			if(buf->data[sensorId] == 0x02) {
				for(int i = 0; i < 4; i++) {
					xvals[i] = buf->data[sensorVal+i];
				}
				xRequest = false;
				xVal = hextofloat(xvals);
				if(buf -> data[sensorId + 1] == 17) {
					xVal = xVal * -1;
				}
			 }
		}
		if(yRequest) {
			if(buf->data[sensorId] == 0x02) {
				for(int i = 0; i < 4; i++) {
					yvals[i] = buf->data[sensorVal+i];
				}
				yRequest = false;
				yVal = hextofloat(yvals);
				if(buf -> data[sensorId + 1] == 17) {
					yVal = yVal * -1;
				}
			}
		}
		if(zRequest) {
			if(buf->data[sensorId] == 0x02) {
				for(int i = 0; i < 4; i++) {
					zvals[i] = buf->data[sensorVal+i];
				}
				zRequest = false;
				zVal = hextofloat(zvals);
			}
		}
		if(butRequest) {
			if(buf->data[sensorId] == 0x02) {
				butVal = buf->data[sensorVal];
				butRequest = false;
			}
		}
		if(ledRequest) {
			if(buf->data[sensorId] == 0x02) {
				requestLed[4] = 0;
        		requestLed[5] = 0;
        		requestLed[6] = 0;
				ledRequest = false;
			}
		}
		if(buzzReq) {
			if(buf->data[sensorId] == 0x02) {
				buzzReq = false;
			}
		}
		if(dcReq) {
			if(buf->data[sensorId] == 0x02) {
				dcReq = false;
			}
		}
		if(sampleReq) {
			if(buf->data[sensorId] == 0x02) {
				sampleReq = false;
			}
		}
		if(sampleT || sampleOn) {
			if(buf->data[sensorId] == 0x02) {
				sampleT = false;
				time = k_uptime_get_32();
				uint8_t d1[4] = {0,0,0,0};
				if(sampleCount > 95) {
					sampleCount = 0;
				}
				allData[0][sampleCount] = buf->data[sensorVal - 1];
				allData[1][sampleCount] = buf->data[sensorVal];
				for(int i = 0; i < 4; i++) {
					d1[i] = buf ->data[sensorVal + i + 1];
				}
				allData[4][sampleCount] = hextofloat(d1);
				for(int i = 0; i < 4; i++) {
					d1[i] = buf ->data[sensorVal + i + 5];
				}
				allData[5][sampleCount] = hextofloat(d1);
				for(int i = 0; i < 4; i++) {
					d1[i] = buf ->data[sensorVal + i + 9];
				}
				allData[6][sampleCount] = hextofloat(d1);
				allData[2][sampleCount] = buf->data[sensorVal + 13];
				allData[3][sampleCount] = buf->data[sensorVal + 14];
				for(int i = 0; i < 7; i++) {
					printk("%d:%f\n",i,allData[i][sampleCount]);
				}
				sampleCount++;
			}
		}
		thingyFound = false;
	}
	mfg_data[2]++;
}

/**
 * @brief function which initialises scanning of devices
 * 
 */
void scan_init(void) {
	// set call back function and designated struct
	bt_le_scan_start(&scan_param, scan_cb);
}
void scan_mobile_init(void(*mobile_found)(const bt_addr_le_t *addr, int8_t rssi, uint8_t adv_type, struct net_buf_simple *buf)) {
	bt_le_scan_start(&scan_param,mobile_found );

}
/**
 * @brief function which loads all data and advertises it
 * 
 * @param sensorData1 data to be advertsied 
 * @param start where the data needs to start being inserted
 * @param len length of sensorData1
 */
void load(uint8_t sensorData1[],int start,int len) {
	// counter to keep track of data allocation
	int c = 0;

	// loop stores all data in main data array
	for(int i = start; i < start + len; i++) {
		data[i] = sensorData1[c];
		c++;
	}

	// update advertisement based on new data
	bt_le_adv_update_data(ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
}

/**
 * @brief function which intis the bluetooth advertisement
 * 
 */
void ble_ahu_adv(void) {
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
 * @brief function inits bluetooth
 * 
 */
void bt_init(void) {
	bt_enable(NULL);
}
