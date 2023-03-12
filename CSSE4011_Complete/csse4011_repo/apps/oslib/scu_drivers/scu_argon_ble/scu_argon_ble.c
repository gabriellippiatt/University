#include "scu_argon_ble.h"
uint8_t data[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
struct bt_data ad[] = {BT_DATA(BT_DATA_SVC_DATA32, data,6),};

// scanning data
static uint8_t mfg_data[] = { 0xff, 0xff, 0x00 };

/* Set Scan Response data */
static const struct bt_data sd[] = {
	BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
};
/**
function that initalises bluetooth
**/
void bt_init(void) {
	bt_enable(NULL);
}

void load(double sensorData1[],int start,int len) {
	int c = 0;
	for(int i = start; i < start + len; i++) {
			data[i] = (uint8_t)sensorData1[c];
			c++;
	}
	bt_le_adv_update_data(ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));

}

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
    printk("addr:%s\n",addr_s);
}
