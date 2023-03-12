#include "scu_uart.h"
void uart_init(void)
{

    const struct device *console_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_console));
    uint32_t dtr = 0;

    /* Enable the USB Driver */
    if (usb_enable(NULL))   
        return;

   /* Wait on DTR - 'Data Terminal Ready'
    * Will wait here until a terminal has been attached to the device
    * This is not necessary, however, can be useful for printing boot info etc..
    */
    while (!dtr) {
        uart_line_ctrl_get(console_dev, UART_LINE_CTRL_DTR, &dtr);
        k_sleep(K_MSEC(100));
    }
}