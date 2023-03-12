#include "ahu_uart.h"
#include "ahu_ble.h"
#define STACK_SIZE 1024
#define THREAD_PRIOR 3
#define THREAD_DELAY 0
#define OPTIONS 0
#define dataBegin 6
#define RSSI_I1 4
#define RSSI_I2 5
#define RSSI_I3 6
#define RSSI_I4 7
#define us11 8
#define us12 10
#define timeS 12

struct static_packet {
	uint16_t usvals1;
    uint16_t usvals2;
    bool devicesFound[4];
    int8_t rssiVals[12];
    uint16_t timer;
};
//static packet
struct static_packet packet;
/* Define a Message Queue */
K_MSGQ_DEFINE(my_msgq, sizeof(struct static_packet), 10, 4);

void scan_thread(void);
void print_thread(void);
/* Thread Prototypes */
K_THREAD_DEFINE(scan_thread_tid, STACK_SIZE, scan_thread, NULL, NULL, NULL, THREAD_PRIOR, OPTIONS, THREAD_DELAY);
K_THREAD_DEFINE(print_thread_tid, STACK_SIZE, print_thread, NULL, NULL, NULL, THREAD_PRIOR, OPTIONS, 100);




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

void device_scancb(const bt_addr_le_t *addr, int8_t rssi, uint8_t adv_type, struct net_buf_simple *buf) {
    uint8_t thingyad[6] = {0xEF,0x92,0x55,0xBB,0x95,0xFE};
    if(find_device(thingyad, addr)) {
        for(int i = 0; i < 12; i++) {
            packet.rssiVals[i] = buf->data[RSSI_I1 + i] * -1;
        }
        packet.timer = (buf->data[timeS] << 8) | buf->data[timeS + 1];
        //printk("us:%d,rs:%d\n",packet.usvals1,packet.rssi_val1);
        k_sleep(K_MSEC(500));
        if (k_msgq_put(&my_msgq, &packet, K_NO_WAIT) != 0) {
		 	k_msgq_purge(&my_msgq);
        }
    }
    

}


void scan_thread(void) {
    bt_init();
    scan_mobile_init(device_scancb);
    while(1) {
         k_sleep(K_MSEC(500));
    }

}

void print_thread(void) {
    uart_init();
    struct static_packet data;
    while(1) {
         if (k_msgq_get(&my_msgq, &data, K_FOREVER) == 0) {
             for(int i = 0; i < 12; i++) {
                 printk("%d:%d,",data.rssiVals[i],data.timer);
             }
             printk("\n");
         }
         k_sleep(K_MSEC(1000));
    }
}