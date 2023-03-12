#include "scu_us.h"
#include "scu_uart.h"
#include "scu_ble.h"

// Initialise STACK SIZE for threads
#define STACK_SIZE 1024
// Set Priority
#define PRIORITY 3
#define SLEEP_TIME_MS   500
struct data_packet {
	uint16_t us;
};

/* Define a Message Queue */
K_MSGQ_DEFINE(my_msgq, sizeof(struct data_packet), 10, 4);

/* Define Consumer Thread */
void us_thread(void);
void adv_thread(void);
#define STACK_SIZE 1024
#define THREAD_PRIOR 3
#define THREAD_DELAY 0
#define OPTIONS 0
K_THREAD_DEFINE(us_thread_tid, STACK_SIZE, us_thread, NULL, NULL, NULL, THREAD_PRIOR, OPTIONS, THREAD_DELAY);
K_THREAD_DEFINE(adv_thread_tid, STACK_SIZE, adv_thread, NULL, NULL, NULL, THREAD_PRIOR, OPTIONS, 100);



void us_thread(void) {
    struct data_packet packet;
    // delete uart init as it hangs the program until someone connects
    //uart_init();
    us_init();
    while(1) {
        packet.us = get_range();
        if (k_msgq_put(&my_msgq, &packet, K_NO_WAIT) != 0) {
			k_msgq_purge(&my_msgq);
		}
        k_sleep(K_MSEC(100));
    }
}


void adv_thread(void) {
    bt_init();
    ble_adv();
    struct data_packet data;
    uint16_t range = 0;
    double ranges[2];
    while(1) {
        if (k_msgq_get(&my_msgq, &data, K_FOREVER) == 0) {
                range = data.us;
                ranges[0] = range >> 8;
                ranges[1] = range & 0x00FF;
                load(ranges,4,2);
				printk("MSG Received: %d\n", data.us);
			memset(&data, 0, sizeof(struct data_packet));
		}
         k_sleep(K_MSEC(100));
	}
}
