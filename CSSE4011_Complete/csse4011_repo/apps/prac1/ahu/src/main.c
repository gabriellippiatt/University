/*
*************************************************************************
* @file oslib/ahu_drivers/ahu/src/main.c
* @author Gabriel Lippiatt - 45296710
* @date 1-04-2021
* @brief button functionality
*************************************************************************
* EXTERNAL FUNCTIONS
*************************************************************************
*************************************************************************
*/
#include "ahu_cli.h"
#include "ahu_ble.h"
#include "ahu_button.h"

//set stack size and priority
#define STACK_SIZE 1024
#define PRIORITY 3
/* Define logging module */
LOG_MODULE_REGISTER(sample_module);

// functions which run the three threads used
void adv_entry(void *, void *, void *);
void scan_entry(void *, void *, void *);
void cli_entry(void *, void *, void *);


// define the three threads used
K_THREAD_DEFINE(adv_tid, STACK_SIZE,
		adv_entry, NULL, NULL, NULL,
		PRIORITY, 0, 100);

K_THREAD_DEFINE(scn_tid, STACK_SIZE,
		scan_entry, NULL, NULL, NULL,
		PRIORITY, 120, 0);

K_THREAD_DEFINE(cli_tid, STACK_SIZE,
		cli_entry, NULL, NULL, NULL,
		PRIORITY - 1, 0, 0);


// define the semaphorses used
K_SEM_DEFINE(scan,0,1);
//start advertiseing task first
K_SEM_DEFINE(adver,1,1);

K_SEM_DEFINE(cli_sem,1,1);

char JSON[7][300];

void adv_entry(void *arg1, void *arg2, void *arg3) {
    //initialise bluetooth
    bt_init();
    //initialise bluetooth advertisement
    ble_ahu_adv();
    button_init();
    while(1) {
        temp = tempVal;
        voc = vocVal;
        humidity = humVal;
        airpressure = apVal;
        xaccel = xVal;
        yaccel = yVal;
        zaccel = zVal;
        button = butVal;
        allRequest = allOn;
        sampleOn = false;
        // check if advetise semaphore has been relased by scan_entry
        if (k_sem_take(&adver, K_SECONDS(10)) != 0) {
            return;
        }
        // check if sensors / other packets have been requested
        if(tempRequest) {
            load(requestTempSensor,2,4);
        }
        if(humRequest) {
            load(requestHumSensor,2,4);
        }
        if(apRequest) {
            load(requestAirPressure,2,4);
        }
        if(vocRequest) {
            load(requestVOC,2,4);
        }
        if(xRequest) {
            load(requestX,2,4);
        }
        if(yRequest) {
            load(requestY,2 ,4);
        }
        if(zRequest) {
            load(requestZ,2 ,4);
        }
        if(butRequest) {
            load(requestBut,2,4);
        }
        if(ledRequest) {
            if(redOn) {
                requestLed[4] = 1;
            }
            if(blueOn) {
                requestLed[6] = 1;                
            }
            if(greenOn) {
                requestLed[5] = 1;
            }
            load(requestLed,2,7);
        }
        // check if an IO,sample rate or Duty Cycle has been set
        if(buzzReq) {
            requestBuzz[4] = buzzerFreq >> 8;
            requestBuzz[5] = buzzerFreq & 0x00FF;
            load(requestBuzz,2,6);
        }
        if(dcReq) {
            requestDC[4] = dcVal;
            load(requestDC, 2 , 5);
        }
        if(sampleReq) {
            requestSample[4] = sampleTime >> 8;
            requestSample[5] = sampleTime & 0x00FF;
            load(requestSample, 2, 6);
        }
        // if button has been pressed and sample mode is on, as well as sample time has passed
        if(k_uptime_get_32() - time >= sampleTime * 1000 && pressed && !sampleT) {
            sampleOn = true;
        }
        // load packet to ask for all data to the scu
        if((sampleOn || sampleT) && pressed) {
            load(requestAll,2,4);
        }

        // if no sensors have been requested advertise default packet (all 0's)
        if(!tempRequest && !humRequest && !apRequest && !vocRequest && !xRequest && !yRequest && !zRequest && 
            !butRequest && !buzzReq && !ledRequest && !dcReq && !sampleReq && !sampleT && !sampleOn) {
            load(defAddy,2,7);
        }
        // give scan and cli sems
        k_sem_give(&scan);
        k_sem_give(&cli_sem);
        // delay task for 200ms
        k_sleep(K_MSEC(200));
        

    }
}

/**
* function that handles scanning
* @param arg1 - thread param
*@param arg2 - thread param
*@param arg3 - thread param
**/
void scan_entry(void *arg1, void *arg2, void *arg3) {
    while(1) {
        // check if scanning sem has been given by advertising(i.e advertising has been completed(for now))
        if (k_sem_take(&scan, K_SECONDS(10)) != 0) {
            return;
        }
        // commence scanning
        scan_init();
        // allow for advertising to commence
        k_sem_give(&adver);
        // don't return to task for 100ms
        k_sleep(K_MSEC(100));
    }

}

/**
* function that handles cli task
* @param arg1 - thread param
*@param arg2 - thread param
*@param arg3 - thread param
**/
void cli_entry(void *arg1, void *arg2, void *arg3) {
    cli_init();
    while(1) {
        // see if avertsing has been completed and cli task can commmence
         if (k_sem_take(&cli_sem, K_SECONDS(10)) != 0) {
            return;
        }
        //check if sensors have been requested or values need to be sent
        tempRequest = (tempCli == true) ? true : tempRequest;
        tempCli = false;
        vocRequest = (vocCli == true)  ? true : vocRequest;
        vocCli = false;
        humRequest = (humCli == true) ? true : humRequest;
        humCli = false;
        apRequest = (apCli == true) ? true : apRequest;
        apCli = false;
        xRequest = (xCli == true) ? true : xRequest;
        xCli = false;
        yRequest = (yCli == true) ? true : yRequest;
        yCli = false;
        zRequest = (zCli == true) ? true : zRequest;
        zCli = false;

        // check if commands with I/O have been requested
        butRequest = (buttonCli == true) ? true : butRequest;
        buttonCli = false;
        ledRequest = (ledCom == true) ? true : ledRequest;
        ledCom = false;
        buzzReq = (buzz == true) ? true : buzzReq;
        buzz = false;
        dcReq = (dc == true) ? true : dcReq;
        dc = false;
        sampleReq = (sample == true) ? true : sampleReq;
        sample = false;     
        sampleT = (allOn == true) ? true : sampleT;
        allOn = false;
        sampleT = (allOff == true) ? false: sampleT;
        sampleReq = (allOff == true) ? false: sampleReq;
        allOff = false;
        
        // don't return to task for 200ms
        k_sleep(K_MSEC(200));
    }
}


