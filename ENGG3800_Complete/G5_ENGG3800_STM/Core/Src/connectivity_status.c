/*
 * connectivity_status.c
 *
 *  Created on: 25 Oct. 2021
 *      Author: jpmcp
 */
#include "connectivity_status.h"

void connectivity_status()
{
    if ((HAL_GetTick() - time2 > 1000)) {
        Connected = 0;
    } else {
        Connected = 1;
    }
}
