/*
 * touch_checker.c
 *
 *  Created on: 25 Oct. 2021
 *      Author: jpmcp
 */
#include "touch_checker.h"
//function used to check if any buttons are being pressed
void touch_checker()
{
    modecontrol();
    holdcontrol();
    resetcontrol();
    brightnesscontrol();
}
