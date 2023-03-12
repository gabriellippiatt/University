/*
 * score.c
 *
 * Written by Peter Sutton
 */

#include "score.h"
#include "terminalio.h"
#include <avr/pgmspace.h>
#include <stdio.h>

uint32_t score;
uint8_t lives;
uint8_t start = 0;

volatile uint8_t seven_seg_cc = 0;

/* Seven segment display segment values for 0 to 9 */
uint8_t seven_seg_data[10] = {63,6,91,79,102,109,125,7,127,111};
	
void game_has_started(void) {
	start = 1;
}

uint8_t get_game_status(void) {
	return start;
}


void init_score(void) {
	score = 0;
	lives = 4;
	move_cursor(25,10);
	printf_P(PSTR("Score: %3d"), get_score());
	move_cursor(25,12);
	printf_P(PSTR("Lives: %3d"), get_lives());
	game_has_started();
}

void add_to_score(uint16_t value) {
	score += value;
	move_cursor(25,10);
	printf_P(PSTR("Score: %3d"), get_score());
}

void loose_a_life(uint8_t value) {
	lives -= value;
	move_cursor(25,12);
	printf_P(PSTR("Lives: %3d"), get_lives());
}

void update_seven_seg(uint8_t digit) {
	DDRC = 0xFF;	// seven seg
	DDRD |= 1 << DDRD3;  // cc toggle

	if (digit == 0) {
		PORTC = seven_seg_data[(score)%10];
	}  else if (digit == 1) {
		if (get_score() >= 10) {
			PORTC = seven_seg_data[(score/10)%10];
			} else {
			PORTC = 0;
		}
		} else {
		PORTC = 0;
	}
	PORTD = digit << DDRD3;
}

uint32_t get_score(void) {
	return score;
}

uint32_t get_lives(void) {
	return lives;
}

void health_bar(void) {
	DDRA |= (1<<0)|(1<<1)|(1<<2)|(1<<3);
	
	if (lives == 4) {
		PORTA = (1<<0)|(1<<1)|(1<<2)|(1<<3);
	} else if (lives == 3) {
		PORTA = (1<<0)|(1<<1)|(1<<2);
	} else if (lives == 2) {
		PORTA = (1<<1)|(1<<2);
	} else if (lives == 1) {
		PORTA = (1<<2);
	} else {
		PORTA = 0;
	}
}
