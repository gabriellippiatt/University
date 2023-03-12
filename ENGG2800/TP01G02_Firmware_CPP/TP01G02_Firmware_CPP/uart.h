/*
 * uart.h
 *
 * Created: 21/08/2019 3:36:44 PM
 *  Author: s4357807
 */ 


#ifndef UART_H_
#define UART_H_
#include "definitions.h"
#include <avr/io.h>

class UART {
	public:
	static void init(void);
	static void transmit( char data );
	static void print(char* str);
	static void println(char* str);
};



#endif /* UART1_H_ */