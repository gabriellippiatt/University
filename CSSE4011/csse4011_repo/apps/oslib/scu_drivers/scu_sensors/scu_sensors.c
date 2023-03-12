/*
*************************************************************************
* @file oslib/scu_drivers/scu_io/scu_sensors.c
* @author Fardeen Bhuiyan - 45296635
* @date 1-04-2021
* @brief Sensor functionality
*************************************************************************
* EXTERNAL FUNCTIONS
*************************************************************************
* void htts221_read(void);
* void lis2dh_read(void);
* void lps22hb_read(void);
* void VOC_READ(void);
*************************************************************************
*/
#include "scu_sensors.h"

//  packet which stores all sensor data
double sensorData[7];

/**
*function which processes temp and humidity sensor
**/
static void process_sample(const struct device *dev) {
	static unsigned int obs;
	struct sensor_value temp, hum;
	if (sensor_sample_fetch(dev) < 0) {
		printk("Sensor sample update error\n");
		return;
	}

	if (sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &temp) < 0) {
		printk("Cannot read HTS221 temperature channel\n");
		return;
	}

	if (sensor_channel_get(dev, SENSOR_CHAN_HUMIDITY, &hum) < 0) {
		printk("Cannot read HTS221 humidity channel\n");
		return;
	}

	++obs;
	printf("Observation:%u\n", obs);

	/* display temperature */
	printf("Temperature:%.1f C\n", sensor_value_to_double(&temp));

	/* display humidity */
	printf("Relative Humidity:%.1f%%\n",sensor_value_to_double(&hum));
	sensorData[0] = sensor_value_to_double(&temp);
	sensorData[1] = sensor_value_to_double(&hum);
}

/**
*call back for temperature sensor
**/
static void hts221_handler(const struct device *dev,
			   const struct sensor_trigger *trig) {
	process_sample(dev);
}

/**
*function which reads temp value
**/
void htts221_read(void) {
	const struct device *dev = device_get_binding("HTS221");

	if (dev == NULL) {
		printf("Could not get HTS221 device\n");
		return;
	}

	if (IS_ENABLED(CONFIG_HTS221_TRIGGER)) {
		struct sensor_trigger trig = {
			.type = SENSOR_TRIG_DATA_READY,
			.chan = SENSOR_CHAN_ALL,
		};
		if (sensor_trigger_set(dev, &trig, hts221_handler) < 0) {
			printf("Cannot configure trigger\n");
			return;
		}
	}
	process_sample(dev);

}

/**
*function which reads the sensor values
*@param sensor - sensor id
**/
static void fetch_and_display(const struct device *sensor) {
	static unsigned int count;
	struct sensor_value accel[3];
	struct sensor_value temperature;
	const char *overrun = "";
	int rc = sensor_sample_fetch(sensor);

	++count;
	if (rc == -EBADMSG) {
		/* Sample overrun.  Ignore in polled mode. */
		if (IS_ENABLED(CONFIG_LIS2DH_TRIGGER)) {
			overrun = "[OVERRUN] ";
		}
		rc = 0;
	}
	if (rc == 0) {
		rc = sensor_channel_get(sensor,
					SENSOR_CHAN_ACCEL_XYZ,
					accel);
	}
	if (rc < 0) {
		printf("ERROR: Update failed: %d\n", rc);
	} else {
		printf("#%u @ %u ms: %sx %f , y %f , z %f",
		       count, k_uptime_get_32(), overrun,
		       sensor_value_to_double(&accel[0]),
		       sensor_value_to_double(&accel[1]),
		       sensor_value_to_double(&accel[2]));
		sensorData[2] = sensor_value_to_double(&accel[0]);
		sensorData[3] = sensor_value_to_double(&accel[1]);
		sensorData[4] = sensor_value_to_double(&accel[2]);
	}

	if (IS_ENABLED(CONFIG_LIS2DH_MEASURE_TEMPERATURE)) {
		if (rc == 0) {
			rc = sensor_channel_get(sensor, SENSOR_CHAN_DIE_TEMP, &temperature);
			if (rc < 0) {
				printf("\nERROR: Unable to read temperature:%d\n", rc);
			} else {
				printf(", t %f\n", sensor_value_to_double(&temperature));
			}
		}
	} else {
		printf("\n");
	}
}

#ifdef CONFIG_LIS2DH_TRIGGER
static void trigger_handler(const struct device *dev,
			    const struct sensor_trigger *trig)
{
	fetch_and_display(dev);
}
#endif

/**
function which accelerator values
**/
void lis2dh_read(void)
{
	const struct device *sensor = DEVICE_DT_GET_ANY(st_lis2dh);

	if (sensor == NULL) {
		printf("No device found\n");
		return;
	}
	if (!device_is_ready(sensor)) {
		printf("Device %s is not ready\n", sensor->name);
		return;
	}

#if CONFIG_LIS2DH_TRIGGER
	{
		struct sensor_trigger trig;
		int rc;

		trig.type = SENSOR_TRIG_DATA_READY;
		trig.chan = SENSOR_CHAN_ACCEL_XYZ;

		if (IS_ENABLED(CONFIG_LIS2DH_ODR_RUNTIME)) {
			struct sensor_value odr = {
				.val1 = 1,
			};

			rc = sensor_attr_set(sensor, trig.chan,
					     SENSOR_ATTR_SAMPLING_FREQUENCY,
					     &odr);
			if (rc != 0) {
				printf("Failed to set odr: %d\n", rc);
				return;
			}
			printf("Sampling at %u Hz\n", odr.val1);
		}

		rc = sensor_trigger_set(sensor, &trig, trigger_handler);
		if (rc != 0) {
			printf("Failed to set trigger: %d\n", rc);
			return;
		}

		printf("Waiting for triggers\n");
		while (true) {
			k_sleep(K_MSEC(2000));
		}
	}
#else /* CONFIG_LIS2DH_TRIGGER */
	fetch_and_display(sensor);
#endif /* CONFIG_LIS2DH_TRIGGER */
}

/**
* process the air pressure Data
*@param dev - device id
*/
static void process_airpressure(const struct device *dev)
{
	static unsigned int obs;
	struct sensor_value pressure, temp;

	if (sensor_sample_fetch(dev) < 0) {
		printf("Sensor sample update error\n");
		return;
	}

	if (sensor_channel_get(dev, SENSOR_CHAN_PRESS, &pressure) < 0) {
		printf("Cannot read LPS22HB pressure channel\n");
		return;
	}

	if (sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &temp) < 0) {
		printf("Cannot read LPS22HB temperature channel\n");
		return;
	}

	++obs;
	printf("Observation:%u\n", obs);

	/* display pressure */
	printf("Pressure:%.1f kPa\n", sensor_value_to_double(&pressure));
	sensorData[5] = sensor_value_to_double(&pressure);

	/* display temperature */
	printf("Temperature:%.1f C\n", sensor_value_to_double(&temp));

}

/**
*read air presseure value
*/
void lps22hb_read(void)
{
	const struct device *dev = device_get_binding(DT_LABEL(DT_INST(0, st_lps22hb_press)));

	if (dev == NULL) {
		printf("Could not get LPS22HB device\n");
		return;
	}

	process_airpressure(dev);
}

static bool app_fw_2;

/**
*helper function that converts the time
*/
static const char *now_str(void)
{
	static char buf[16]; /* ...HH:MM:SS.MMM */
	uint32_t now = k_uptime_get_32();
	unsigned int ms = now % MSEC_PER_SEC;
	unsigned int s;
	unsigned int min;
	unsigned int h;

	now /= MSEC_PER_SEC;
	s = now % 60U;
	now /= 60U;
	min = now % 60U;
	now /= 60U;
	h = now;

	snprintf(buf, sizeof(buf), "%u:%02u:%02u.%03u",
		 h, min, s, ms);
	return buf;
}

static int do_fetch(const struct device *dev) {
	struct sensor_value co2, tvoc, voltage, current;
	int rc = 0;
#ifdef CONFIG_APP_MONITOR_BASELINE
	rc = ccs811_baseline_fetch(dev);
	if (rc >= 0) {
		baseline = rc;
		rc = 0;
	}
#endif
	if (rc == 0) {
		rc = sensor_sample_fetch(dev);
	}
	if (rc == 0) {
		const struct ccs811_result_type *rp = ccs811_result(dev);

		sensor_channel_get(dev, SENSOR_CHAN_CO2, &co2);
		sensor_channel_get(dev, SENSOR_CHAN_VOC, &tvoc);
		sensor_channel_get(dev, SENSOR_CHAN_VOLTAGE, &voltage);
		sensor_channel_get(dev, SENSOR_CHAN_CURRENT, &current);
		printk("\n[%s]: CCS811: %u ppm eCO2; %u ppb eTVOC\n",
		       now_str(), co2.val1, tvoc.val1);
		sensorData[6] = tvoc.val1;
		printk("Voltage: %d.%06dV; Current: %d.%06dA\n", voltage.val1,
		       voltage.val2, current.val1, current.val2);
#ifdef CONFIG_APP_MONITOR_BASELINE
		printk("BASELINE %04x\n", baseline);
#endif
		if (app_fw_2 && !(rp->status & CCS811_STATUS_DATA_READY)) {
			printk("STALE DATA\n");
		}

		if (rp->status & CCS811_STATUS_ERROR) {
			printk("ERROR: %02x\n", rp->error);
		}
	}
	return rc;
}

#ifndef CONFIG_CCS811_TRIGGER_NONE

/**
*handler for the VOC
*@param dev - device id
*@param trig - sensor trigger
*/
static void trigger_handler_CCS811(const struct device *dev,
			    const struct sensor_trigger *trig) {
	int rc = do_fetch(dev);

	if (rc == 0) {
		printk("Triggered fetch got %d\n", rc);
	} else if (-EAGAIN == rc) {
		printk("Triggered fetch got stale data\n");
	} else {
		printk("Triggered fetch failed: %d\n", rc);
	}
}

#endif /* !CONFIG_CCS811_TRIGGER_NONE */
/**
* function that fetcs VOC
* @param dev -device id
*/
static void do_main(const struct device *dev) {
	int rc = do_fetch(dev);

	if (rc == 0) {
		printk("Timed fetch got %d\n", rc);
	} else if (-EAGAIN == rc) {
		printk("Timed fetch got stale data\n");
	} else {
		printk("Timed fetch failed: %d\n", rc);
	}
}

/**
*function that reads voc
*/
void VOC_READ(void) {
	const struct device *dev = device_get_binding(DT_LABEL(DT_INST(0, ams_ccs811)));
	struct ccs811_configver_type cfgver;
	int rc;

	if (!dev) {
		printk("Failed to get device binding");
		return;
	}

	printk("device is %p, name is %s\n", dev, dev->name);

	rc = ccs811_configver_fetch(dev, &cfgver);
	if (rc == 0) {
		printk("HW %02x; FW Boot %04x App %04x ; mode %02x\n",
		       cfgver.hw_version, cfgver.fw_boot_version,
		       cfgver.fw_app_version, cfgver.mode);
		app_fw_2 = (cfgver.fw_app_version >> 8) > 0x11;
	}

#ifdef CONFIG_APP_USE_ENVDATA
	struct sensor_value temp = { CONFIG_APP_ENV_TEMPERATURE };
	struct sensor_value humidity = { CONFIG_APP_ENV_HUMIDITY };

	rc = ccs811_envdata_update(dev, &temp, &humidity);
	printk("ENV_DATA set for %d Cel, %d %%RH got %d\n",
	       temp.val1, humidity.val1, rc);
#endif

#ifdef CONFIG_CCS811_TRIGGER
	struct sensor_trigger trig = { 0 };

#ifdef CONFIG_APP_TRIGGER_ON_THRESHOLD
	printk("Triggering on threshold:\n");
	if (rc == 0) {
		struct sensor_value thr = {
			.val1 = CONFIG_APP_CO2_MEDIUM_PPM,
		};
		rc = sensor_attr_set(dev, SENSOR_CHAN_CO2,
				     SENSOR_ATTR_LOWER_THRESH,
				     &thr);
		printk("L/M threshold to %d got %d\n", thr.val1, rc);
	}
	if (rc == 0) {
		struct sensor_value thr = {
			.val1 = CONFIG_APP_CO2_HIGH_PPM,
		};
		rc = sensor_attr_set(dev, SENSOR_CHAN_CO2,
				     SENSOR_ATTR_UPPER_THRESH,
				     &thr);
		printk("M/H threshold to %d got %d\n", thr.val1, rc);
	}
	trig.type = SENSOR_TRIG_THRESHOLD;
	trig.chan = SENSOR_CHAN_CO2;
#elif defined(CONFIG_APP_TRIGGER_ON_DATAREADY)
	printk("Triggering on data ready\n");
	trig.type = SENSOR_TRIG_DATA_READY;
	trig.chan = SENSOR_CHAN_ALL;
#else
#error Unhandled trigger on
#endif
	if (rc == 0) {
		rc = sensor_trigger_set(dev, &trig, trigger_handler);
	}
	if (rc == 0) {
#ifdef CONFIG_APP_TRIGGER_ON_DATAREADY
		while (true) {
			k_sleep(K_FOREVER);
		}
#endif
	}
	printk("Trigger installation got: %d\n", rc);
#endif /* CONFIG_CCS811_TRIGGER */
	if (rc == 0) {
		do_main(dev);
	}
}
