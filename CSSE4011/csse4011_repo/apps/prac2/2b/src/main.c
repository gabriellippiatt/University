/*
*************************************************************************
* @file oslib/scu_drivers/scu/src/main.c
* @author Fardeen Bhuiyan - 45296635
* @date 1-04-2021
* @brief button functionality
*************************************************************************
* EXTERNAL FUNCTIONS
*************************************************************************
*************************************************************************
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "scu_ble.h"
#define STACK_SIZE 1024
#define THREAD_PRIOR 3
#define THREAD_DELAY 0
#define OPTIONS 0
#define usBegin 6
struct static_packet {
	uint8_t usvals1[2];
    uint8_t usvals2[2];
    bool devicesFound[4];
    int8_t rssiVals[12];
    int8_t deviceCount;
};

struct timer {
    uint16_t time;
};

struct beacons {
    uint8_t addy[6];
    bool found;
    int deviceNum;
    bool first;
};
uint16_t timeVal = 0;
struct beacons beams[15];
uint8_t alladdys[15][6] = {{0x67,0x34,0x85,0xFE,0x75,0xF5},{0x86,0x1E,0x06,0x87,0x73,0xE5},
{0xB1,0x98,0xFD,0x9E,0x99,0xCA},{0xFE,0xFF,0x82,0x89,0x1B,0xCB},{0x04,0x25,0xFF,0x57,0xBD,0xF9},{0x7C,0xB7,0xE9,0x27,0x13,0xC1}
,{0xA0,0x39,0x06,0x48,0x04,0xF1},{0x60,0xCE,0xDB,0xE0,0x0C,0xCA},{0x13,0x20,0x7C,0xD4,0x7F,0xD4},{0xE1,0xC8,0xF1,0x21,0x0B,0xF7}
,{0x4A,0x3E,0xFA,0x8D,0xE0,0xFD},{0xAC,0xFA,0x28,0xF7,0x32,0xEE},{0x39,0x5F,0x44,0x7E,0x1A,0xD8}, {0x9A,0x7A,0xC2,0x07, 0x7F, 0xE5},
{0x34,0xBA,0x11,0xDF,0x0D,0xFE}};

struct static_packet packet;
bool stSet = false;
/* Define a Message Queue */
K_MSGQ_DEFINE(my_msgq, sizeof(struct static_packet), 10, 4);
K_MSGQ_DEFINE(my_mstime, sizeof(struct timer), 10, 4);
void scan_thread(void);
void adv_thread(void);
void time_thread(void);
/* Thread Prototypes */
K_THREAD_DEFINE(scan_thread_tid, STACK_SIZE, scan_thread, NULL, NULL, NULL, THREAD_PRIOR, OPTIONS, THREAD_DELAY);
K_THREAD_DEFINE(adv_thread_tid, STACK_SIZE, adv_thread, NULL, NULL, NULL, THREAD_PRIOR, OPTIONS, 100);
K_THREAD_DEFINE(time_thread_tid, STACK_SIZE, time_thread, NULL, NULL, NULL, THREAD_PRIOR, OPTIONS, 100);



 void init_beacons(void) {
     for(int i = 0 ; i < 15; i++) {
         beams[i].first = true;
     }
 }
void reset_beacons(void) {
    for(int i = 0; i < 15; i++) {
        for(int j = 0; j < 6; j++) {
            beams[i].addy[j] = alladdys[i][j];
        }
        beams[i].found = false;
    }

}

void reset_devices(void) {
    for(int i = 0; i < 4; i++) {
        packet.devicesFound[i] = false;
    }
    packet.deviceCount = 0;
}

bool find_device(uint8_t addy[],const bt_addr_le_t *addr) {
    for(int i = 0; i < 6; i++) {
        if(addy[i] == addr->a.val[i]) {
            continue;
        }
        else {
            return false;
        }
    }
    return true;
}

// void find_beam(const bt_addr_le_t *addr, int8_t rssi) {
//     for(int i = 0; i < 15; i++) {
//         if(find_device(beams[i].addy,addr) && !beams[i].found) {
//             if(!stSet) {
//                 beams[i].deviceNum = 3;
//                 stSet = true;
//             }
//             beams[i].found = true;
//             packet.deviceCount++;
//             if(!packet.devicesFound[2]) {
//                 packet.devicesFound[2] = true;
//                 if(beams[i].deviceNum == 3) {
//                     packet.rssi_val3 = abs(rssi);

//                 }
//                 else {
//                     packet.rssi_val4 = abs(rssi);
//                 }

//             }
//             else {
//                 packet.devicesFound[3] = true;
//                 if(beams[i].deviceNum == 3) {
//                     packet.rssi_val3 = abs(rssi);

//                 }
//                 else {
//                     packet.rssi_val4 = abs(rssi);
//                 }
//             }
//         }
//     }

// }
void device_scancb(const bt_addr_le_t *addr, int8_t rssi, uint8_t adv_type, struct net_buf_simple *buf) {
    uint8_t fadsArgon[6] = {0x68,0x70,0x89,0x63,0xB1,0xF4}; 
    uint8_t gabeArgon[6] = {0x92,0xC4,0xB7,0x6E,0x06,0xED}; // replace with actual

    if(find_device(fadsArgon,addr)) {
        packet.deviceCount++;
        packet.rssiVals[0] = abs(rssi);
    
    }
    if(find_device(alladdys[1],addr)) {
        packet.deviceCount++;
        packet.rssiVals[1] = abs(rssi);
    
     }
     if(find_device(alladdys[2],addr)) {
        packet.deviceCount++;
        packet.rssiVals[2] = abs(rssi);

     }
    if(find_device(alladdys[3],addr)) {
        packet.deviceCount++;
        packet.rssiVals[0] = abs(rssi);
    
    }
    if(find_device(alladdys[4],addr)) {
        packet.deviceCount++;
        packet.rssiVals[1] = abs(rssi);
    
     }
     if(find_device(alladdys[5],addr)) {
        packet.deviceCount++;
        packet.rssiVals[2] = abs(rssi);

     }
     if(find_device(alladdys[6],addr)) {
        packet.deviceCount++;
        packet.rssiVals[2] = abs(rssi);

     }
    if(find_device(alladdys[7],addr)) {
        packet.deviceCount++;
        packet.rssiVals[7] = abs(rssi);
    
    }
    if(find_device(alladdys[8],addr)) {
        packet.deviceCount++;
        packet.rssiVals[8] = abs(rssi);
    
     }
     if(find_device(alladdys[9],addr)) {
        packet.deviceCount++;
        packet.rssiVals[9] = abs(rssi);

     }
    if(find_device(alladdys[10],addr)) {
        packet.deviceCount++;
        packet.rssiVals[10] = abs(rssi);
    
    }
    if(find_device(alladdys[11],addr)) {
        packet.deviceCount++;
        packet.rssiVals[11] = abs(rssi);
    
     }
  
    //find_beam(addr,rssi);
    //if(packet.deviceCount == 4) {
        if (k_msgq_put(&my_msgq, &packet, K_NO_WAIT) != 0) {
            packet.deviceCount = 0;
		 	k_msgq_purge(&my_msgq);
        }
   // }


}

void scan_thread(void) {
    bt_init();
    scan_argon_init(device_scancb);
    reset_devices();
    reset_beacons();
    init_beacons();
    while(1) {
         k_sleep(K_MSEC(100));
    }


}

void adv_thread(void) {
    ble_adv();
    struct static_packet data;
    struct timer timeRec;
    double adv_packet[14];
    while(1) {
        if (k_msgq_get(&my_msgq, &data, K_FOREVER) == 0) {
            for(int i = 0; i < 12; i++) {
                adv_packet[i] = data.rssiVals[i];
            }
            printk("a:%f,b:%f,c:%f\n",adv_packet[0],adv_packet[1],adv_packet[2]);
        }
         if (k_msgq_get(&my_mstime, &timeRec, K_FOREVER) == 0) {
             printk("time:%d\n",timeRec.time);
             adv_packet[12] = timeRec.time >> 8;
             adv_packet[13] = timeRec.time & 0x00FF;
         }
        load2b(adv_packet,2,14);
        reset_devices();
        reset_beacons();
        k_sleep(K_MSEC(100));
    }
}

void time_thread(void) {
    struct timer timeSec;
    while(1) {
        timeVal++;
        timeSec.time = timeVal;
        if (k_msgq_put(&my_mstime, &timeSec, K_NO_WAIT) != 0) {

        }
        k_sleep(K_MSEC(1000));
    }
}