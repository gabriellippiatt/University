/*
*************************************************************************
* @file oslib/scu_drivers/scu_speaker/scu_speaker.h
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
#ifndef SCU_SPEAKER
#define SCU_SPEAKER
#include <drivers/regulator.h>
#include <drivers/pwm.h>
#define SPK_PWR_NODE	DT_NODELABEL(spk_pwr)
void speaker_init(void);
void speaker_set(int freq);

#endif