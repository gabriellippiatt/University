/*
*************************************************************************
* @file oslib/ahu_drivers/ahu_cli/ahu_cli.c
* @author Gabriel Lippiatt - 45296710
* @date 1-04-2021
* @brief cli functionality
*************************************************************************
* EXTERNAL FUNCTIONS
*************************************************************************
*void cli_init(void)
*************************************************************************
*/

#include "ahu_cli.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Define logging module */
LOG_MODULE_REGISTER(cli_module);

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   500

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

#if DT_NODE_HAS_STATUS(LED0_NODE, okay)
#define LED0	DT_GPIO_LABEL(LED0_NODE, gpios)
#define PIN	DT_GPIO_PIN(LED0_NODE, gpios)
#define FLAGS	DT_GPIO_FLAGS(LED0_NODE, gpios)
#else
/* A build error here means your board isn't set up to blink an LED. */
#error "Unsupported board: led0 devicetree alias is not defined"
#define LED0	""
#define PIN	0
#define FLAGS	0
#endif

// Define LED 1 (RED)
#define LED1_NODE DT_ALIAS(led1)
#define LED1	DT_GPIO_LABEL(LED1_NODE, gpios)
#define PIN1	DT_GPIO_PIN(LED1_NODE, gpios)
#define FLAGS1	DT_GPIO_FLAGS(LED1_NODE, gpios)

// Define LED 2 (GREEN)
#define LED2_NODE DT_ALIAS(led2)
#define LED2	DT_GPIO_LABEL(LED2_NODE, gpios)
#define PIN2	DT_GPIO_PIN(LED2_NODE, gpios)
#define FLAGS2	DT_GPIO_FLAGS(LED2_NODE, gpios)

// Define LED 3 (BLUE)
#define LED3_NODE DT_ALIAS(led3)
#define LED3	DT_GPIO_LABEL(LED3_NODE, gpios)
#define PIN3	DT_GPIO_PIN(LED3_NODE, gpios)
#define FLAGS3	DT_GPIO_FLAGS(LED3_NODE, gpios)

/* Declare command handler prototypes */
static int cmd_led_ctrl_on(const struct shell *, size_t, char **);
static int cmd_led_ctrl_off(const struct shell *, size_t, char **);
static int cmd_led_ctrl_toggle(const struct shell *, size_t, char **);
static int cmd_time_seconds(const struct shell *, size_t, char **);
static int cmd_time_formatted(const struct shell *, size_t, char **);

/* Declarations for the SCU Command interface */
static int cmd_hts221_read(const struct shell *, size_t, char **);
static int cmd_lps22_read(const struct shell *, size_t, char **);
static int cmd_ccs811_read(const struct shell *, size_t, char **);
static int cmd_lis2dh_read(const struct shell *, size_t, char **);
static int cmd_buzzer_ctrl(const struct shell *, size_t, char **);
static int cmd_rgb_ctrl(const struct shell *, size_t, char **);
static int cmd_pb_read(const struct shell *, size_t, char **);
static int cmd_dc_ctrl(const struct shell *, size_t, char **);
static int cmd_sample_ctrl(const struct shell *, size_t, char **);
static int cmd_sample_all_on(const struct shell *, size_t, char **);
static int cmd_sample_all_off(const struct shell *, size_t, char **);

// Status of the LEDs (all LEDs initialised OFF - See CLI Init)
int led1_status = 0;
int led2_status = 0;
int led3_status = 0;

bool vocCli = false;;
bool tempCli = false;
bool humCli = false;
bool apCli = false;
bool xCli = false;
bool yCli = false;
bool zCli = false;
bool redOn = false;
bool blueOn = false;
bool greenOn = false;
bool ledCom = false;
bool allOn = false;
bool allOff = false;
bool buzz = false;
bool dc  = false;
bool sample = false;


bool buttonCli = false;
uint8_t temp = 0;
uint8_t voc = 0;
uint8_t humidity = 0;
uint8_t airpressure = 0;
float xaccel = 0;
float yaccel = 0;
float zaccel = 0;
uint8_t  button = 0;
uint16_t sampleTime = 0;
uint16_t buzzerFreq = 0;
uint8_t dcVal = 0;

/* Specify Shell Commands for LED Toggling */
/* Creating subcommands (level 1 command) array for command "led". */ 
SHELL_STATIC_SUBCMD_SET_CREATE(led_ctrl,
        SHELL_CMD(o, NULL, "Turn led on.", cmd_led_ctrl_on),
        SHELL_CMD(f,   NULL, "Turn led off.", cmd_led_ctrl_off),
		SHELL_CMD(t,   NULL, "Toggle led on/off.", cmd_led_ctrl_toggle),
        SHELL_SUBCMD_SET_END
); 



/* Specify Shell Commands for LED Toggling */
/* Creating subcommands (level 1 command) array for command "led". */ 
SHELL_STATIC_SUBCMD_SET_CREATE(time_ctrl,
        SHELL_CMD(f, NULL, "Formatted Time", cmd_time_formatted),
        SHELL_SUBCMD_SET_END
); 

/* Specify Shell Commands for reading temperature and humidity */
SHELL_STATIC_SUBCMD_SET_CREATE(hts221_read,
        SHELL_CMD(r, NULL, "Read from hts221", cmd_hts221_read),
        SHELL_SUBCMD_SET_END
); 

/* Specify Shell Commands for reading pressure */
SHELL_STATIC_SUBCMD_SET_CREATE(lps22_read,
        SHELL_CMD(r, NULL, "Read from lps22", cmd_lps22_read),
        SHELL_SUBCMD_SET_END
); 

/* Specify Shell Commands for reading VOC */
SHELL_STATIC_SUBCMD_SET_CREATE(ccs811_read,
        SHELL_CMD(r, NULL, "Read from ccs811", cmd_ccs811_read),
        SHELL_SUBCMD_SET_END
); 

/* Specify Shell Commands for reading X,Y,Z values of the accelerometer */
SHELL_STATIC_SUBCMD_SET_CREATE(lis2dh_read,
        SHELL_CMD(r, NULL, "Read from lis2dh", cmd_lis2dh_read),
        SHELL_SUBCMD_SET_END
); 

/* Specify Shell Commands for controlling buzzer frequency */
SHELL_STATIC_SUBCMD_SET_CREATE(buzzer_ctrl,
        SHELL_CMD(w, NULL, "Write to buzzer", cmd_buzzer_ctrl),
        SHELL_SUBCMD_SET_END
); 

/* Specify Shell Commands for controlling RGB LEDs */
SHELL_STATIC_SUBCMD_SET_CREATE(rgb_ctrl,
        SHELL_CMD(w, NULL, "Write to rgb", cmd_rgb_ctrl),
        SHELL_SUBCMD_SET_END
); 

/* Specify Shell Commands for reading the Push Button State */
SHELL_STATIC_SUBCMD_SET_CREATE(pb_read,
        SHELL_CMD(r, NULL, "Read from pb", cmd_pb_read),
        SHELL_SUBCMD_SET_END
); 

/* Specify Shell Commands for controlling Duty Cycle (percentage on) */
SHELL_STATIC_SUBCMD_SET_CREATE(dc_ctrl,
        SHELL_CMD(w, NULL, "Write to dc", cmd_dc_ctrl),
        SHELL_SUBCMD_SET_END
); 

/* Specify Shell Commands for controlling sampling time */
SHELL_STATIC_SUBCMD_SET_CREATE(sample_ctrl,
        SHELL_CMD(w, NULL, "Write to sample", cmd_sample_ctrl),
        SHELL_SUBCMD_SET_END
); 

/* Specify Shell Commands for controlling sampling all */
SHELL_STATIC_SUBCMD_SET_CREATE(sample_all,
        SHELL_CMD(o, NULL, "Sample All ON", cmd_sample_all_on),
		SHELL_CMD(f, NULL, "Sample All OF", cmd_sample_all_off),
        SHELL_SUBCMD_SET_END
); 


/* Creating root (level 0) command "time" */
SHELL_CMD_REGISTER(time, &time_ctrl, "Time Unformatted", cmd_time_seconds);

/* Creating root (level 0) command "led" */
SHELL_CMD_REGISTER(led, &led_ctrl, "LED Control", NULL);

/* Creating root (level 0) command "hts221" */
SHELL_CMD_REGISTER(hts221, &hts221_read, "hts221 Read", NULL);

/* Creating root (level 0) command "lps22" */
SHELL_CMD_REGISTER(lps22, &lps22_read, "lps22 Read", NULL);

/* Creating root (level 0) command "ccs811" */
SHELL_CMD_REGISTER(ccs811, &ccs811_read, "ccs811 Read", NULL);

/* Creating root (level 0) command "lis2dh" */
SHELL_CMD_REGISTER(lis2dh, &lis2dh_read, "lis2dh Read", NULL);

/* Creating root (level 0) command "buzzer" */
SHELL_CMD_REGISTER(buzzer, &buzzer_ctrl, "buzzer Control", NULL);

/* Creating root (level 0) command "rgb" */
SHELL_CMD_REGISTER(rgb, &rgb_ctrl, "rgb Control", NULL);

/* Creating root (level 0) command "pb" */
SHELL_CMD_REGISTER(pb, &pb_read, "pb Read", NULL);

/* Creating root (level 0) command "dc" */
SHELL_CMD_REGISTER(dc, &dc_ctrl, "dc Control", NULL);

/* Creating root (level 0) command "sample" */
SHELL_CMD_REGISTER(sample, &sample_ctrl, "sample Control", NULL);

/* Creating root (level 0) command "all" */
SHELL_CMD_REGISTER(all, &sample_all, "sample all", NULL);


/* Command Handler for turning LEDs ON */
static int cmd_led_ctrl_on(const struct shell *shell, size_t argc,
                        char **argv)
{
        ARG_UNUSED(argc);

	const struct device *dev1 = device_get_binding(LED1);
	const struct device *dev2 = device_get_binding(LED2);
	const struct device *dev3 = device_get_binding(LED3);

	if (dev2 == NULL) {
		return ENODEV;
	}

	if (strcmp(argv[1],"r") == 0) {
		if (led1_status == 0) {
			led1_status = 1;
			LOG_INF("red led is on");
			return gpio_pin_set(dev1, PIN1, 1);
		} else {
			LOG_WRN("red led is already on");
			return 0;
		}
	} else if (strcmp(argv[1],"g") == 0) {
		if (led2_status == 0) {
			led2_status = 1;
			LOG_INF("green led is on");
			return gpio_pin_set(dev2, PIN2, 1);
		} else {
			LOG_WRN("green led is already on");
			return 0;
		}
	} else if (strcmp(argv[1],"b") == 0) {
		if (led3_status == 0) {
			led3_status = 1;
			LOG_INF("blue led is on");
			return gpio_pin_set(dev3, PIN3, 1);
		} else {
			LOG_WRN("blue led is already on");
			return 0;
		}
	} else {
		LOG_ERR("invalid command");
		return 0;
	}
	
}

/* Command Handler for turning LEDs OFF */
static int cmd_led_ctrl_off(const struct shell *shell, size_t argc,
                        char **argv)
{
        ARG_UNUSED(argc);

	const struct device *dev1 = device_get_binding(LED1);
	const struct device *dev2 = device_get_binding(LED2);
	const struct device *dev3 = device_get_binding(LED3);

	if (dev2 == NULL) {
		return ENODEV;
	}

	if (strcmp(argv[1],"r") == 0) {
		if (led1_status == 1) {
			led1_status = 0;
			LOG_INF("red led is off");
			return gpio_pin_set(dev1, PIN1, 0);
		} else {
			LOG_WRN("red led is already off");
			return 0;
		}
	} else if (strcmp(argv[1],"g") == 0) {
		if (led2_status == 1) {
			led2_status = 0;
			LOG_INF("green led is off");
			return gpio_pin_set(dev2, PIN2, 0);
		} else {
			LOG_WRN("green led is already off");
			return 0;
		}
	} else if (strcmp(argv[1],"b") == 0) {
		if (led3_status == 1) {
			led3_status = 0;
			LOG_INF("blue led is off");
			return gpio_pin_set(dev3, PIN3, 0);
		} else {
			LOG_WRN("blue led is already off");
			return 0;
		}
	} else {
		LOG_ERR("invalid command");
		return 0;
	}
}

/* Command Handler for toggling LEDs */
static int cmd_led_ctrl_toggle(const struct shell *shell, size_t argc,
                        char **argv)
{
        ARG_UNUSED(argc);

	const struct device *dev1 = device_get_binding(LED1);
	const struct device *dev2 = device_get_binding(LED2);
	const struct device *dev3 = device_get_binding(LED3);

	if (dev2 == NULL) {
		return ENODEV;
	}

	if (strcmp(argv[1],"r") == 0) {
		if (led1_status == 0) {
			led1_status = 1;
			LOG_INF("red led is on");
			return gpio_pin_set(dev1, PIN1, 1);
		} else {
			led1_status = 0;
			LOG_INF("red led is off");
			return gpio_pin_set(dev1, PIN1, 0);
		}
	} else if (strcmp(argv[1],"g") == 0) {
		if (led2_status == 0) {
			led2_status = 1;
			LOG_INF("green led is on");
			return gpio_pin_set(dev2, PIN2, 1);
		} else {
			led2_status = 0;
			LOG_INF("green led is off");
			return gpio_pin_set(dev2, PIN2, 0);
		}
	} else if (strcmp(argv[1],"b") == 0) {
		if (led3_status == 0) {
			led3_status = 1;
			LOG_INF("blue led is on");
			return gpio_pin_set(dev3, PIN3, 1);
		} else {
			led3_status = 0;
			LOG_INF("blue led is off");
			return gpio_pin_set(dev3, PIN3, 0);
		}
	} else {
		LOG_ERR("invalid command");
		return 0;
	}
}

/* Command Handler for displaying the time (unformatted) */
static int cmd_time_seconds(const struct shell *shell, size_t argc,
                        char **argv)
{
        ARG_UNUSED(argc);

	char time_str[80];
	uint32_t time_int;

	if (strcmp(argv[1],NULL) != 0) {
		LOG_ERR("Invalid Parameter. Should be NULL for unfromatted time");
		return 0;
	} else {

	sprintf(time_str, "%d", k_uptime_get_32());

	time_int = atoi(time_str);

	int seconds = (int) ((time_int / 1000));

	shell_print(shell,"%d",seconds);

	return 0;
	}
}

/* Command Handler for displaying the time (formatted) */
static int cmd_time_formatted(const struct shell *shell, size_t argc,
                        char **argv)
{
        ARG_UNUSED(argc);
        ARG_UNUSED(argv);

	char time_str[80];
	uint32_t time_int;

	sprintf(time_str, "%d", k_uptime_get_32());

	time_int = atoi(time_str);

	int seconds = (int) ((time_int / 1000) % 60);
	int minutes = (int) ((time_int / (1000*60)) % 60);
	int hours   = (int) ((time_int / (1000*60*60)) % 24);

	shell_print(shell,"%02d:%02d:%02d", hours, minutes, seconds);

	return 0;
}

/* Command Handler for displaying the hts221 */
static int cmd_hts221_read(const struct shell *shell, size_t argc,
                        char **argv)
{
        ARG_UNUSED(argc);

	if (strcmp(argv[1],"t") == 0) {
		tempCli = true;
		k_sleep(K_MSEC(waitTime));
		shell_print(shell,"%d C\n",temp);
		LOG_INF("Read Temperature");
	} else if (strcmp(argv[1],"h") == 0) {
		humCli = true;
		k_sleep(K_MSEC(waitTime));
		shell_print(shell,"%d %%\n",humidity);
		LOG_INF("Read Humidity");
	} else {
		LOG_ERR("invalid command");
	}
	return 0;
}

/* Command Handler for displaying the lps22 */
static int cmd_lps22_read(const struct shell *shell, size_t argc,
                        char **argv)
{
        ARG_UNUSED(argc);

	apCli = true;
	k_sleep(K_MSEC(waitTime));
	shell_print(shell,"%d kPA\n",airpressure);

	LOG_INF("Read Pressure");
	return 0;
}

/* Command Handler for displaying the ccs811 */
static int cmd_ccs811_read(const struct shell *shell, size_t argc,
                        char **argv)
{
        ARG_UNUSED(argc);

	vocCli = true;
	k_sleep(K_MSEC(waitTime));
	shell_print(shell,"%d atm\n",voc);

	LOG_INF("Read VOC");
	return 0;
}

/* Command Handler for displaying the lis2dh */
static int cmd_lis2dh_read(const struct shell *shell, size_t argc,
                        char **argv)
{
        ARG_UNUSED(argc);

	if (strcmp(argv[1],"x") == 0) {
		xCli = true;
		k_sleep(K_MSEC(waitTime));
		shell_print(shell,"%f m/s^2\n",xaccel);
		LOG_INF("Read X Acceleration");
	} else if (strcmp(argv[1],"y") == 0) {
		yCli = true;
		k_sleep(K_MSEC(waitTime));
		shell_print(shell,"%f m/s^2\n",yaccel);
		LOG_INF("Read Y Acceleration");
	} else if (strcmp(argv[1],"z") == 0) {
		zCli = true;
		k_sleep(K_MSEC(waitTime));
		shell_print(shell,"%f m/s^2\n",zaccel);
		LOG_INF("Read Z Acceleration");
	} else {
		LOG_ERR("invalid command");
	}
	return 0;
}

/* Command Handler for displaying the buzzer */
static int cmd_buzzer_ctrl(const struct shell *shell, size_t argc,
                        char **argv)
{
        ARG_UNUSED(argc);
		buzz = true;
		buzzerFreq = atoi(argv[1]);

	LOG_INF("Control Buzzer");
	return 0;
}

/* Command Handler for displaying the rgb */
static int cmd_rgb_ctrl(const struct shell *shell, size_t argc,
                        char **argv)
{
        ARG_UNUSED(argc);
		ledCom = true;
		if(argv[1][0] == 'o') {
			redOn = true;
		} else if(argv[1][0] == 'f') {
			redOn = false;
		} else if(argv[1][0] == 't') {
			redOn = !redOn;
		} else {
			LOG_ERR("Invalid Command");
			return 0;
		}			
		if(argv[1][1] == 'o') {
			greenOn = true;
		} else if(argv[1][1] == 'f') {
			greenOn = false;
		} else if(argv[1][1] == 't') {
			 greenOn= !greenOn;
		} else {
			LOG_ERR("Invalid Command");
			return 0;
		}
		if(argv[1][2] == 'o') {
			blueOn = true;
		} else if(argv[1][2] == 'f') {
			blueOn = false;
		} else if(argv[1][2] == 't') {
			 blueOn = !blueOn;
		} else {
			LOG_ERR("Invalid Command");
			return 0;
		}
	LOG_INF("Control RGB");
	return 0;
}

/* Command Handler for displaying the pb */ //////////////////////////////// clarify if the button is toggled or held
static int cmd_pb_read(const struct shell *shell, size_t argc,
                        char **argv)
{
        ARG_UNUSED(argc);
		buttonCli = true;
		k_sleep(K_MSEC(waitTime));
		shell_print(shell,"%d\n",button);

	LOG_INF("Read PB");
	return 0;
}

/* Command Handler for displaying the dc */
static int cmd_dc_ctrl(const struct shell *shell, size_t argc,
                        char **argv)
{
        ARG_UNUSED(argc);
		dc = true;
		dcVal = atoi(argv[1]);
	LOG_INF("Control DC");
	return 0;
}

/* Command Handler for displaying the sample */
static int cmd_sample_ctrl(const struct shell *shell, size_t argc,
                        char **argv)
{
        ARG_UNUSED(argc);
		sample = true;
		sampleTime = atoi(argv[1]);

	LOG_INF("Control sample");
	return 0;
}

/* Command Handler for displaying the sample all on */
static int cmd_sample_all_on(const struct shell *shell, size_t argc,
                        char **argv)
{
        ARG_UNUSED(argc);
		allOn = true;

	LOG_INF("sample all on");

	return 0;
}

/* Command Handler for displaying the sample all on */
static int cmd_sample_all_off(const struct shell *shell, size_t argc,
                        char **argv)
{
        ARG_UNUSED(argc);
		allOn = false;
		allOff = true;

	LOG_INF("sample all off");

	return 0;
}

/*
 * Sample app to init USB, and demonstrate shell functionality
 */
void cli_init(void)
{
	const struct device *dev1;
	const struct device *dev2;
	const struct device *dev3;
	int ret1;
	int ret2;
	int ret3;

	/* Setup DTR */
	const struct device *shell_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_shell_uart));
	uint32_t dtr = 0;
	
	if (usb_enable(NULL))
		return;

	/* Wait on DTR - 'Data Terminal Ready'
	 * Will wait here until a terminal has been attached to the device
	 * This is not necessary, however, can be useful from reading early data
	 *
	 * Remove the following while loop if DTR is not required!!
	 */
	
	while (!dtr) {
		uart_line_ctrl_get(shell_dev, UART_LINE_CTRL_DTR, &dtr);
		k_sleep(K_MSEC(100));
	} 

	/* DTR - OK, Continue */

	dev1 = device_get_binding(LED1);
	if (dev1 == NULL) {
		return;
	}
	dev2 = device_get_binding(LED2);
	if (dev2 == NULL) {
		return;
	}
	dev3 = device_get_binding(LED3);
	if (dev3 == NULL) {
		return;
	}

	ret1 = gpio_pin_configure(dev1, PIN1, GPIO_OUTPUT_ACTIVE | FLAGS1);
	if (ret1 < 0) {
		return;
	}
	ret2 = gpio_pin_configure(dev2, PIN2, GPIO_OUTPUT_ACTIVE | FLAGS2);
	if (ret2 < 0) {
		return;
	}
	ret3 = gpio_pin_configure(dev3, PIN3, GPIO_OUTPUT_ACTIVE | FLAGS3);
	if (ret3 < 0) {
		return;
	}

	// Turn off all the LEDs
	gpio_pin_set(dev1, PIN1, 0);
	gpio_pin_set(dev2, PIN2, 0);
	gpio_pin_set(dev3, PIN3, 0);

	LOG_DBG("led init OK");
}