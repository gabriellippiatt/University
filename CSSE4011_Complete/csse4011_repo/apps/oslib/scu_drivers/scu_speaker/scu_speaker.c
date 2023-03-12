/*
*************************************************************************
* @file oslib/scu_drivers/scu_speaker/scu_speaker.c
* @author Fardeen Bhuiyan - 45296635
* @date 1-04-2021
* @brief Sensor functionality
*************************************************************************
* EXTERNAL FUNCTIONS
*************************************************************************
*void speaker_init(void);
*void speaker_set(int freq);
*************************************************************************
*/
#include "scu_speaker.h"
static const struct device* speaker_power = DEVICE_DT_GET(SPK_PWR_NODE);
const struct device *pwm_dev;


void speaker_init(void) {
    pwm_dev = device_get_binding(DT_LABEL(DT_NODELABEL(pwm0)));

	if (!device_is_ready(pwm_dev)) {
		printk("E");
	}
	if (!device_is_ready(speaker_power)) {
		printk("Error");
		return;
	}
	regulator_enable(speaker_power, NULL);
}

void speaker_set(int freq) {
    if(freq == 0) {
        pwm_pin_set_usec(pwm_dev, 0x1b, 0, 0, 0);
        return;
    }
    pwm_pin_set_usec(pwm_dev, 0x1b, USEC_PER_SEC/freq, ((USEC_PER_SEC/freq) / 2U), 0);
}