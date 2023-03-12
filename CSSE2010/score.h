/*
 * score.h
 * 
 * Author: Peter Sutton
 */

#ifndef SCORE_H_
#define SCORE_H_

#include <stdint.h>

void game_has_started(void);
uint8_t get_game_status(void);
void init_score(void);
void add_to_score(uint16_t value);
void loose_a_life(uint8_t value);
void update_seven_seg(uint8_t digit);
uint32_t get_score(void);
uint32_t get_lives(void);
void health_bar(void);


#endif /* SCORE_H_ */