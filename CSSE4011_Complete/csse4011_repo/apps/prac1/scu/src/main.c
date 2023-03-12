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
#include "scu_sensors.h"
#include "scu_ble.h"
#include "scu_button.h"
#include "scu_led.h"
#include "scu_speaker.h"
// Initialise STACK SIZE for threads
#define STACK_SIZE 1024
// Set Priority
#define PRIORITY 3

/* Thread Prototypes */
void sensor_entry(void *, void *, void *);
void bt_entry(void *, void *, void *);
void scan_entry(void *, void *, void *);


// Threads defined 
K_THREAD_DEFINE(sensor_tid, STACK_SIZE,
		sensor_entry, NULL, NULL, NULL,
		PRIORITY, 0, 100);

// Threads defined (compile time)
K_THREAD_DEFINE(bt_tid, STACK_SIZE,
		bt_entry, NULL, NULL, NULL,
		PRIORITY, 0, 100);


K_THREAD_DEFINE(scn_tid, STACK_SIZE,
		scan_entry, NULL, NULL, NULL,
		PRIORITY, 0, 0);


// /* Define Semaphores */
K_SEM_DEFINE(sensor, 0, 1);
K_SEM_DEFINE(bt, 1, 1);
K_SEM_DEFINE(scan,1,1);
K_SEM_DEFINE(adver,0,1);

// flag to check if a sensor value has been changed
bool set = false;
// gloabal accel struct to convert float to hex value
uint8_t xyzVals[4];
uint8_t butVal = 0x00;
uint16_t freq = 0;
uint16_t sampler = 0;

/**
* function which convers float to hex array
*@param val - float value to be convert to hex
*@param vals- array to store hex value
**/
unsigned char* fltohex(float val, uint8_t vals[])
{
// union to store float and hex val
   union 
   {
      float          f;
      unsigned char  b[sizeof(float)];
   } v = { val };
   size_t i;
   printf("%f is stored as ", v.f);
   // store the float into a hex array
   for ( i = 0; i < 4; ++i )
   {
        vals[i] = v.b[i];
        printf("%02X%c", v.b[i], i < sizeof(v.b) - 1 ? '-' : '\n');
   }
   // return hex array
   return vals  ;
}


/**
*function that polls sensor values at a sampled rate, which is also dependent
*on duty cycle
* @param arg1 - thread param
*@param arg2 - thread param
*@param arg3 - thread param
*/
void sensor_entry(void *arg1, void *arg2, void *arg3) {
    while(1) {
        htts221_read();
        lps22hb_read();
        VOC_READ();
        lis2dh_read();
        printk("yooy%f %d\n,",(dutyVal/100),sampler);
        k_sleep(K_MSEC((sampler*1000)*(1-(dutyVal/100)) +200));
    }
}


/**
*function that handles bt advertising
* @param arg1 - thread param
*@param arg2 - thread param
*@param arg3 - thread param
*/
void bt_entry(void *arg1, void *arg2, void *arg3) {
    ble_adv();
    button_init();
    led_init();
    speaker_init();
    while(1) {
        if (k_sem_take(&adver, K_SECONDS(10)) != 0) {
            return;
        }
        set = false;
        // check if a value has been request by dongle
        if(loadTemp) {
            loadTemp = false;
            set = true;
            sensorAdv[3] = sensorData[0];
            load(sensorAdv,2,4);
        }
        if(loadHum) {
            loadHum = false;
            set = true;
            sensorAdv[3] = sensorData[1];
            load(sensorAdv,2,4);
        }

        if(loadAp) {
            loadAp = false;
            set = true;
            sensorAdv[3] = sensorData[5];
            load(sensorAdv,2,4);
        }

        if(loadVoc) {
            loadVoc = false;
            set = true;
            sensorAdv[3] = sensorData[6];
            load(sensorAdv,2,4);

        }

        if(loadX) {
            loadX = false;
            set = true;
            fltohex(sensorData[2],xyzVals);
            accelAdv[3] = xyzVals[0];
            accelAdv[4] = xyzVals[1];
            accelAdv[5] = xyzVals[2];
            accelAdv[6] = xyzVals[3];
           load(accelAdv,2,7);

        }
        if(loadY) {
            loadY = false;
            set = true;
            fltohex(sensorData[3],xyzVals);
            accelAdv[3] = xyzVals[0];
            accelAdv[4] = xyzVals[1];
            accelAdv[5] = xyzVals[2];
            accelAdv[6] = xyzVals[3];
           load(accelAdv,2,7);

        }
        if(loadZ) {
            loadZ = false;
            set = true;
            fltohex(sensorData[4],xyzVals);
            accelAdv[3] = xyzVals[0];
            accelAdv[4] = xyzVals[1];
            accelAdv[5] = xyzVals[2];
            accelAdv[6] = xyzVals[3];
            load(accelAdv,2,7);

        }
        // checks if an I/O has been requested
        if(loadBut) {
            if(pressed) {
                butVal = 0x01;
            }
            else {
                butVal = 0x00;
            }
            loadBut = false;
            set = true;
            sensorAdv[3] = butVal;
            load(sensorAdv,2,4);
         }
        if(ledRGB) {
            led_red(redVal);
            led_green(greenVal);
            led_blue(blueVal);
            load(sensorAdv,2,4);
            set = true;
            ledRGB = false;

        }
        if(buzz) {
            buzz = false;
            set = true;
            freq = (buzzFH << 8) | buzzFL;
            printk("buz%d\n",freq);
            speaker_set(freq);
            load(sensorAdv,2,4);
        }
        // checks if a duty cycle or sample rate has been set
        if(retDC) {
            load(sensorAdv,2,4);
            retDC = false;
            set = true;
        }
        if(retSample) {
            sampler = (sampleH << 8) | sampleL;
            retSample = false;
            set = true;
            load(sensorAdv,2,4);
        }
        // checks if sampling mode is on and all data has been requested
        if(loadAll) {
            allAdv[2] = sensorData[0];
            allAdv[3] = sensorData[1];
            fltohex(sensorData[2],xyzVals);
            allAdv[4] = xyzVals[0];
            allAdv[5] = xyzVals[1];
            allAdv[6] = xyzVals[2];
            allAdv[7] = xyzVals[3];
            fltohex(sensorData[3],xyzVals);
            allAdv[8] = xyzVals[0];
            allAdv[9] = xyzVals[1];
            allAdv[10] = xyzVals[2];
            allAdv[11] = xyzVals[3];
            fltohex(sensorData[4],xyzVals);
            allAdv[12] = xyzVals[0];
            allAdv[13] = xyzVals[1];
            allAdv[14] = xyzVals[2];
            allAdv[15] = xyzVals[3];
            
            allAdv[16] = sensorData[5];
            allAdv[17] = sensorData[6];
            loadAll = false;
            set = true;
            load(allAdv,2,18);
        }
        // advertise 0's if nothing has been requested
        if(!loadTemp && !loadHum && !loadAp && !loadVoc && !loadX && !loadY && !loadZ && !loadBut && !ledRGB 
            && !buzz && !set && !retDC && !retSample && !loadAll) {
            load(defAd,2,18);
        }
        k_sem_give(&scan);
        k_sleep(K_MSEC(1000*(1-(dutyVal/100))+1000));
        

    }
}


/**
* functions which handles scanning 
* @param arg1 - thread param
*@param arg2 - thread param
*@param arg3 - thread param
**/
void scan_entry(void *arg1, void *arg2, void *arg3) {
    // initialise blue tooth
    bt_init();
    while(1) {
        if (k_sem_take(&scan, K_SECONDS(10)) != 0) {
            return;
        }
        // initalise scanning
        scanner_init();
        k_sem_give(&adver);
        k_sleep(K_MSEC(500*(1-(dutyVal/100))+500));
    }

}
