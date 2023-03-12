/*
 * CPPFile1.cpp
 *
 * Created: 21/08/2019 3:36:10 PM
 *  Author: s4357807
 */ 
#include "uart.h"



void UART::init() {
	UBRR0H = (uint8_t)((UBRR_VAL & 0xFF00) >> 8);
	UBRR0L = (uint8_t) UBRR_VAL & 0xFF;
	//Enable receiver, transmitter and receive complete interrupt
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
	//Set frame format: 8 data bits, 1 even parity bit, 2 stop bits
	UCSR0C = (1<<UPM01)|(1<<USBS0)|(3<<UCSZ00);
	sei();
}

void UART::transmit( char data ) {
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) );
	asm volatile ("nop");
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

void UART::print(char* str) {
	if(str != 0) {
		uint8_t i = 0;
		char chr;
		while((chr = str[i++]) != 0) {
			transmit(chr);
		}
	}
}

void UART::println(char* str) {
	print(str);
	transmit('\r');
	transmit('\n');
}