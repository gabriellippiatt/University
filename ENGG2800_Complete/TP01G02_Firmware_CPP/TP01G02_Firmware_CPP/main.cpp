/*
 * TP01G02_Firmware_CPP.cpp
 *
 * Created: 21/08/2019 10:58:10 AM
 * Author : s4357807
 */ 

////////////////////////////////// Includes //////////////////////////////////


#include <avr/io.h>
#include <stdint-gcc.h>
#include <stdlib.h>
#include "epd2in9.h"
#include "epdpaint.h"
#include "imagedata.h"
#include "uart.h"

#define COLORED     0
#define UNCOLORED   1



////////////////////////////////// Type Declarations ////////////////////////


/**
  * Due to RAM not enough in Arduino UNO, a frame buffer is not allowed.
  * In this case, a smaller image buffer is allocated and you have to 
  * update a partial display several times.
  * 1 byte = 8 pixels, therefore you have to set 8*N pixels at a time.
  */
unsigned char image[1024];
Paint paint(image, 0, 0);    // width should be the multiple of 8 
Epd epd;
uint8_t serial_cursor = 0;
char serial_buffer[64]; 



void setup() {
	// put your setup code here, to run once:
    UART::init();
	if (epd.Init(lut_full_update) != 0) {
		UART::println("e-Paper init failed");
		return;
	}
	
	

	/** 
	*  there are 2 memory areas embedded in the e-paper display
	*  and once the display is refreshed, the memory area will be auto-toggled,
	*  i.e. the next action of SetFrameMemory will set the other memory area
	*  therefore you have to clear the frame memory twice.
	*/
	epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
	epd.DisplayFrame();
	epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
	epd.DisplayFrame();


	if (epd.Init(lut_full_update) != 0) {
		UART::println("e-Paper init failed");
		return;
	}
}

void loop() {
	paint.SetRotate(ROTATE_0);
	paint.SetWidth(128);
	paint.SetHeight(32);

	paint.Clear(UNCOLORED);
	paint.DrawStringAt(0, 0, serial_buffer, &Font8_2, COLORED);
	epd.SetFrameMemory(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());

	
	paint.SetWidth(65);
	paint.SetHeight(65);
	
	paint.Clear(UNCOLORED);
	paint.DrawRectangle(0, 0, 64, 64, COLORED);
	epd.SetFrameMemory(paint.GetImage(), 32, 116, paint.GetWidth(), paint.GetHeight());
	epd.DisplayFrame();
	epd.SetFrameMemory(paint.GetImage(), 32, 116, paint.GetWidth(), paint.GetHeight());
	epd.DisplayFrame();
	
	paint.Clear(UNCOLORED);
	for(uint8_t i = 0; i < 64; i++) {
		serial_buffer[i] = 0;
	}
	serial_cursor = 0;
	_delay_ms(2000);
}


int main(void)
{
	/* Replace with your application code */
	UART::init();
	if (epd.Init(lut_full_update) != 0) {
		UART::println("e-Paper init failed");
	}
	setup();
	
	//UBRR0H = (uint8_t)((UBRR_VAL & 0xFF00) >> 8);
	//UBRR0L = (uint8_t) UBRR_VAL & 0xFF;
	//Enable receiver and transmitter
	//UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8 data bits, 1 even parity bit, 2 stop bits */
	//UCSR0C = (1<<UPM01)|(1<<USBS0)|(3<<UCSZ00);
	//while (1) {
		loop();
	//}
	while (1) {
		asm volatile("nop");
	}
	/* Wait for empty transmit buffer */
	//while ( !( UCSR0A & (1<<UDRE0)) );
	//asm volatile ("nop");
	//asm volatile ("nop");
	/* Put data into buffer, sends the data */
	//UDR0 = 'e';
	//UART::println((char*)"Hello World!");
	
}

ISR(USART_RX_vect) {
	cli();
	//char chr = UDR0;
	//serial_buffer[serial_cursor++] = chr;
	UART::transmit(UDR0);
	sei();
}