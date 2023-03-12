/*
 * project.c
 *
 * Main file
 *
 * Author: Peter Sutton. Modified by <YOUR NAME HERE>
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>

#include "ledmatrix.h"
#include "scrolling_char_display.h"
#include "buttons.h"
#include "serialio.h"
#include "terminalio.h"
#include "score.h"
#include "timer0.h"
#include "game.h"

#define F_CPU 8000000L
#include <util/delay.h>

uint32_t score;
uint8_t pause_status;


// Function prototypes - these are defined below (after main()) in the order
// given here
void initialise_hardware(void);
void splash_screen(void);
void new_game(void);
void play_game(void);
void handle_game_over(void);
void initialise_joystic(void);
int16_t get_j_x(void);
int16_t get_j_y(void);
void game_pause(void);

// ASCII code for Escape character
#define ESCAPE_CHAR 27

/////////////////////////////// main //////////////////////////////////
int main(void) {
		
	// Setup hardware and call backs. This will turn on 
	// interrupts.
	initialise_hardware();
	
	// Show the splash screen message. Returns when display
	// is complete
	splash_screen();
	
	while(1) {
		new_game();
		play_game();
		handle_game_over();
	}
}

void initialise_hardware(void) {
	ledmatrix_setup();
	init_button_interrupts();
	// Setup serial port for 19200 baud communication with no echo
	// of incoming characters
	init_serial_stdio(19200,0);
	
	init_timer0();
	
	// Turn on global interrupts
	sei();
}

void splash_screen(void) {
	// Clear terminal screen and output a message
	clear_terminal();
	move_cursor(10,10);
	printf_P(PSTR("Asteroids"));
	move_cursor(10,12);
	printf_P(PSTR("CSSE2010/7201 project by Gabriel Lippiatt 45296710"));
	
	// Output the scrolling message to the LED matrix
	// and wait for a push button to be pushed.
	ledmatrix_clear();
	while(1) {
		set_scrolling_display_text("ASTEROIDS BY 45296710", COLOUR_GREEN);
		// Scroll the message until it has scrolled off the 
		// display or a button is pushed
		while(scroll_display()) {
			_delay_ms(150);
			if(button_pushed() != NO_BUTTON_PUSHED) {
				return;
			}
		}
	}
}

void new_game(void) {
	// Initialise the game and display
	initialise_game();
	
	initialise_joystic();
	
	// Clear the serial terminal
	clear_terminal();
	
	// Initialise the score
	init_score();
	
	// Clear a button push or serial input if any are waiting
	// (The cast to void means the return value is ignored.)
	(void)button_pushed();
	clear_serial_input_buffer();
	
	pause_status = 0;
}

void play_game(void) {
	uint32_t current_time, last_move_time, current_time1, last_move_time1, current_LR_time, last_LR_move_time, current_UD_time, last_UD_move_time;
	int8_t button;
	char serial_input, escape_sequence_char;
	uint8_t characters_into_escape_sequence = 0;
	
	// Get the current time and remember this as the last time the projectiles
    // were moved.
	current_time = get_current_time();
	last_move_time = current_time;
	
	current_time1 = get_current_time();
	last_move_time1 = current_time1;
	
	current_LR_time = get_current_time();
	last_LR_move_time = current_LR_time;
	
	current_UD_time = get_current_time();
	last_UD_move_time = current_UD_time;
	
	// We play the game until it's over
	while(!is_game_over()) {	
			

		
		// Check for input - which could be a button push or serial input.
		// Serial input may be part of an escape sequence, e.g. ESC [ D
		// is a left cursor key press. At most one of the following three
		// variables will be set to a value other than -1 if input is available.
		// (We don't initalise button to -1 since button_pushed() will return -1
		// if no button pushes are waiting to be returned.)
		// Button pushes take priority over serial input. If there are both then
		// we'll retrieve the serial input the next time through this loop
		serial_input = -1;
		escape_sequence_char = -1;
		button = button_pushed();
		
		if(button == NO_BUTTON_PUSHED) {
			// No push button was pushed, see if there is any serial input
			if(serial_input_available()) {
				// Serial data was available - read the data from standard input
				serial_input = fgetc(stdin);
				// Check if the character is part of an escape sequence
				if(characters_into_escape_sequence == 0 && serial_input == ESCAPE_CHAR) {
					// We've hit the first character in an escape sequence (escape)
					characters_into_escape_sequence++;
					serial_input = -1; // Don't further process this character
				} else if(characters_into_escape_sequence == 1 && serial_input == '[') {
					// We've hit the second character in an escape sequence
					characters_into_escape_sequence++;
					serial_input = -1; // Don't further process this character
				} else if(characters_into_escape_sequence == 2) {
					// Third (and last) character in the escape sequence
					escape_sequence_char = serial_input;
					serial_input = -1;  // Don't further process this character - we
										// deal with it as part of the escape sequence
					characters_into_escape_sequence = 0;
				} else {
					// Character was not part of an escape sequence (or we received
					// an invalid second character in the sequence). We'll process 
					// the data in the serial_input variable.
					characters_into_escape_sequence = 0;
				}
			}
		}
		
		// Process the input. 
		if(button==3 || escape_sequence_char=='D' || serial_input=='L' || serial_input=='l') {
			// Button 3 pressed OR left cursor key escape sequence completed OR
			// letter L (lowercase or uppercase) pressed - attempt to move left
			if(pause_status==0){
				move_base(MOVE_LEFT);
			}
		} else if(button==2 || escape_sequence_char=='A' || serial_input==' ') {
			// Button 2 pressed or up cursor key escape sequence completed OR
			// space bar pressed - attempt to fire projectile
			if(pause_status==0){
				fire_projectile();
			}
		} else if(button==1 || escape_sequence_char=='B') {
			// Button 1 pressed OR down cursor key escape sequence completed
			// Ignore at present
			game_pause();
		} else if(button==0 || escape_sequence_char=='C' || serial_input=='R' || serial_input=='r') {
			// Button 0 pressed OR right cursor key escape sequence completed OR
			// letter R (lowercase or uppercase) pressed - attempt to move right
			if(pause_status==0){
				move_base(MOVE_RIGHT);
			}
		} else if(serial_input == 'p' || serial_input == 'P') {
			// Unimplemented feature - pause/unpause the game until 'p' or 'P' is
			// pressed again
			game_pause();
			
		} 
		// else - invalid input or we're part way through an escape sequence -
		// do nothing
		
		if(pause_status == 0){
			current_time = get_current_time();
		}

		if(!is_game_over() && current_time >= last_move_time + 150) {
			// 500ms (0.5 second) has passed since the last time we moved
			// the projectiles - move them - and keep track of the time we 
			// moved them
			advance_projectiles();
			
			last_move_time = current_time;
		}

		
		base_hit_detection();
		health_bar();
		accelerate_asteroids();

	
		if(pause_status==0){
			current_time1 = get_current_time();
		}
		
		if(!is_game_over() && current_time1 >= last_move_time1 + get_speed()) {
			// 500ms (0.5 second) has passed since the last time we moved
			// the projectiles - move them - and keep track of the time we
			// moved them
			
			falling_asteroids();
			
			check_position();	
			
			last_move_time1 = current_time1;
		}
		
		if(pause_status==0){
			current_LR_time = get_current_time();
		}
				
		if(!is_game_over() && current_LR_time >= last_LR_move_time + 150) {
			// 500ms (0.5 second) has passed since the last time we moved
			// the projectiles - move them - and keep track of the time we
			// moved them
					
			if(get_j_x() == -1) {
				move_base(MOVE_LEFT);
			} else if(get_j_x() == 1) {
				move_base(MOVE_RIGHT);
			}
					
			last_LR_move_time = current_LR_time;
		}
		
		if(pause_status==0){
			current_UD_time = get_current_time();
		}
				
		if(!is_game_over() && current_UD_time >= last_UD_move_time + 200) {
			// 500ms (0.5 second) has passed since the last time we moved
			// the projectiles - move them - and keep track of the time we
			// moved them
					
			if(get_j_y() == 1) {
				fire_projectile();
			}
						
			last_UD_move_time = current_UD_time;
		}
	}
	// We get here if the game is over.
}
// initailise the joystic
void initialise_joystic(void) {
	/* Set up the serial port for stdin communication at 19200 baud, no echo */
	//init_serial_stdio(19200,0);
	
	/* Turn on global interrupts */
	//sei();
	
	// Set up ADC - AVCC reference, right adjust
	// Input selection doesn't matter yet - we'll swap this around in the while
	// loop below.
	ADMUX = (1<<REFS0);
	// Turn on the ADC (but don't start a conversion yet). Choose a clock
	// divider of 64. (The ADC clock must be somewhere
	// between 50kHz and 200kHz. We will divide our 8MHz clock by 64
	// to give us 125kHz.)
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1);
}
// get x
int16_t get_j_x(void) {

	ADMUX = (1<<REFS0) | (1<<MUX1) | (1<<MUX2);
	
	ADCSRA |= (1<<ADSC);
	
	while(ADCSRA & (1<<ADSC)) {
		; /* Wait until conversion finished */
	}
	
	int16_t x_value = ADC; // read the value
	
	if(x_value > 700){
		return 1;
	} else if(x_value < 300) {
		return -1;
	} else {
		return 0;
	}
}
// get y
int16_t get_j_y(void) {
	
	ADMUX = (1<<REFS0) | (1<<MUX0) | (1<<MUX1) | (1<<MUX2);
		
	ADCSRA |= (1<<ADSC);
		
	while(ADCSRA & (1<<ADSC)) {
		; /* Wait until conversion finished */
	}
		
	int16_t y_value = ADC; // read the value
		
	if(y_value > 600){
		return 1;
    } else {
		return 0;
	}
}

void game_pause(void) {
	 // If pause_status == 1, game is paused.
	 // If pause_status == 0, game is running.
	 // initially game is running (pause_status = 0).
	 // and when game_pause the status changes.
	 pause_status = 1 ^ pause_status;
}

void handle_game_over() {
	int16_t i, j, k;
	k = 40;
	
	clear_terminal();
	move_cursor(10,12);
	printf_P(PSTR("Your Final Score Was: %d"), get_score());
	move_cursor(10,14);
	printf_P(PSTR("GAME OVER"));
	move_cursor(10,15);
	printf_P(PSTR("Press a button to start again"));
	
	//--------------GAME OVER SEQUENCE--------------------//
	game_pause();
	// Stage 1
	ledmatrix_update_pixel(0,9-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(0,5-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(1,8-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(1,6-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(2,7-get_base_position(), COLOUR_ORANGE);
	_delay_ms(k);
	// Clear Stage 1 and base
	ledmatrix_update_pixel(0,9-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(0,5-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(1,8-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(1,6-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(2,7-get_base_position(), COLOUR_BLACK);
	
	ledmatrix_update_pixel(0,8-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(0,7-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(0,6-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(1,7-get_base_position(), COLOUR_BLACK);
	// Stage 2
	ledmatrix_update_pixel(0,10-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(1,10-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(1,9-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(2,9-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(2,8-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(3,8-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(3,7-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(2,6-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(3,6-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(1,5-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(2,5-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(0,4-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(1,4-get_base_position(), COLOUR_ORANGE);
	_delay_ms(k);
	// Clear Stage 2
	ledmatrix_update_pixel(0,10-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(1,10-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(1,9-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(2,9-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(2,8-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(3,8-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(3,7-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(2,6-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(3,6-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(1,5-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(2,5-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(0,4-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(1,4-get_base_position(), COLOUR_BLACK);
	// Stage 3
	ledmatrix_update_pixel(0,11-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(1,11-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(2,11-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(2,10-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(3,10-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(3,9-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(4,9-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(4,8-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(4,7-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(4,6-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(4,5-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(3,5-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(3,4-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(2,4-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(2,3-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(1,3-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(0,3-get_base_position(), COLOUR_ORANGE);
	_delay_ms(k);
	//Clear Stage 3
	ledmatrix_update_pixel(0,11-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(1,11-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(2,11-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(2,10-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(3,10-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(3,9-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(4,9-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(4,8-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(4,7-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(4,6-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(4,5-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(3,5-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(3,4-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(2,4-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(2,3-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(1,3-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(0,3-get_base_position(), COLOUR_BLACK);
	// Stage 4
	ledmatrix_update_pixel(0,12-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(1,12-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(2,12-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(3,12-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(3,11-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(4,11-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(4,10-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(5,10-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(5,9-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(5,8-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(5,7-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(5,6-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(5,5-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(5,4-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(4,4-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(4,3-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(3,3-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(3,2-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(2,2-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(1,2-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(0,2-get_base_position(), COLOUR_ORANGE);
	_delay_ms(k);
	//Clear Stage 4
	ledmatrix_update_pixel(0,12-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(1,12-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(2,12-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(3,12-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(3,11-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(4,11-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(4,10-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(5,10-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(5,9-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(5,8-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(5,7-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(5,6-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(5,5-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(5,4-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(4,4-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(4,3-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(3,3-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(3,2-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(2,2-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(1,2-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(0,2-get_base_position(), COLOUR_BLACK);
	// Stage 5
	ledmatrix_update_pixel(0,13-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(1,13-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(2,13-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(3,13-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(4,13-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(4,12-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(5,12-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(5,11-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(6,11-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(6,10-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(6,9-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(6,8-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(6,7-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(6,6-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(6,5-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(6,4-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(6,3-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(5,3-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(5,2-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(4,2-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(4,1-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(3,1-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(2,1-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(1,1-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(0,1-get_base_position(), COLOUR_ORANGE);
	_delay_ms(k);
	//Clear Stage 5
	ledmatrix_update_pixel(0,13-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(1,13-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(2,13-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(3,13-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(4,13-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(4,12-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(5,12-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(5,11-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(6,11-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(6,10-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(6,9-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(6,8-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(6,7-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(6,6-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(6,5-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(6,4-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(6,3-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(5,3-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(5,2-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(4,2-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(4,1-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(3,1-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(2,1-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(1,1-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(0,1-get_base_position(), COLOUR_BLACK);
	// Stage 6
	ledmatrix_update_pixel(0,14-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(1,14-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(2,14-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(3,14-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(4,14-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(5,14-get_base_position(), COLOUR_ORANGE);
// 		ledmatrix_update_pixel(6,14-get_base_position(), COLOUR_ORANGE);
// 		ledmatrix_update_pixel(7,14-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(5,13-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(6,13-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(6,12-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(7,12-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(7,11-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(7,10-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(7,9-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(7,8-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(7,7-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(7,6-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(7,5-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(7,4-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(7,3-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(7,2-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(6,2-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(6,1-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(5,1-get_base_position(), COLOUR_ORANGE);
// 			ledmatrix_update_pixel(7,0-get_base_position(), COLOUR_ORANGE);
// 			ledmatrix_update_pixel(6,0-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(5,0-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(4,0-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(3,0-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(2,0-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(1,0-get_base_position(), COLOUR_ORANGE);
	ledmatrix_update_pixel(0,0-get_base_position(), COLOUR_ORANGE);	
	_delay_ms(k);
	ledmatrix_update_pixel(0,14-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(1,14-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(2,14-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(3,14-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(4,14-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(5,14-get_base_position(), COLOUR_BLACK);
			ledmatrix_update_pixel(6,14-get_base_position(), COLOUR_BLACK);
			ledmatrix_update_pixel(7,14-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(5,13-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(6,13-get_base_position(), COLOUR_BLACK);
			ledmatrix_update_pixel(7,13-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(6,12-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(7,12-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(7,11-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(7,10-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(7,9-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(7,8-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(7,7-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(7,6-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(7,5-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(7,4-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(7,3-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(7,2-get_base_position(), COLOUR_BLACK);
			ledmatrix_update_pixel(7,1-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(6,2-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(6,1-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(5,1-get_base_position(), COLOUR_BLACK);
			ledmatrix_update_pixel(7,0-get_base_position(), COLOUR_BLACK);
			ledmatrix_update_pixel(6,0-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(5,0-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(4,0-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(3,0-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(2,0-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(1,0-get_base_position(), COLOUR_BLACK);
	ledmatrix_update_pixel(0,0-get_base_position(), COLOUR_BLACK);
	// End of screen
	for(j=8;j<17;j++){
			for(i=-1;i<15;i++) {
				ledmatrix_update_pixel(j,i-get_base_position(), COLOUR_ORANGE);
			}
			_delay_ms(k);
			for(i=0;i<15;i++) {
				ledmatrix_update_pixel(j,i-get_base_position(), COLOUR_BLACK);
			}
	}
	//------------END OF SEQUENCE-------------------------//
	ledmatrix_clear();
	while(1) {
		set_scrolling_display_text("GAME OVER", COLOUR_RED);
		// Scroll the message until it has scrolled off the
		// display or a button is pushed
		while(scroll_display()) {
			_delay_ms(150);
			if(button_pushed() != NO_BUTTON_PUSHED) {
				return;
			}
		}
	}
	game_pause();	
}
