/*
*************************************************************************
* @file oslib/ahu_drivers/ahu_button/ahu_button.c
* @author Gabriel Lippiatt - 45296710
* @date 1-04-2021
* @brief button functionality
*************************************************************************
* EXTERNAL FUNCTIONS
*************************************************************************
*void button_init(void);
*************************************************************************
*/
#include "ahu_button.h"

#define SW0_NODE	DT_ALIAS(sw0)

LOG_MODULE_REGISTER(button_module);

#if !DT_NODE_HAS_STATUS(SW0_NODE, okay)
#error "Unsupported board: sw0 devicetree alias is not defined"
#endif

static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios,
							      {0});
static struct gpio_callback button_cb_data;

bool pressed = false;

/**
 * @brief 
 * 
 * @param dev 
 * @param cb 
 * @param pins 
 */
void button_pressed(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins) {
   	pressed= !pressed;
	printk("Button pressed at %" PRIu32 "\n", k_cycle_get_32());
}

/**
 * @brief 
 * 
 */
void button_init(void) {
	int ret;

	if (!device_is_ready(button.port)) {
		LOG_ERR("Error: button device %s is not ready\n",
		       button.port->name);
		return;
	}

	ret = gpio_pin_configure_dt(&button, GPIO_INPUT);

	if (ret != 0) {
		LOG_ERR("Error %d: failed to configure %s pin %d\n",
		       ret, button.port->name, button.pin);
		return;
	}

	ret = gpio_pin_interrupt_configure_dt(&button,
					      GPIO_INT_EDGE_TO_ACTIVE);
	if (ret != 0) {
		LOG_ERR("Error %d: failed to configure interrupt on %s pin %d\n",
			ret, button.port->name, button.pin);
		return;
	}

	gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));
	gpio_add_callback(button.port, &button_cb_data);
}
