/*
 * IncFile1.h
 *
 * Created: 21/08/2019 11:10:15 AM
 *  Author: s4357807
 */ 


#ifndef DEFINES_H_
#define DEFINES_H_
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>


////////////////////////////////// Defines //////////////////////////////////

#define EPD_RESET_PIN 0
#define EPD_CS_PIN 1
#define EPD_DC_PIN 2
#define MOSI_PIN 3
#define MISO_PIN 4
#define SCK_PIN 5
#define EPD_BUSY_PIN 6

#define pass 0
#define fail 1
#define PWR_RED_REG PRR
#define SPI_PWR_RED_BIT PRSPI
#define SPI_SHIFT_REGISTER
#define SPI_CONTROL_REGISTER SPCR
#define SPI_DATA_REGISTER SPDR
#define SPI_STATUS_REGISTER SPSR
#define SPI_FINISHED_BIT SPIF
#define SPI_PORT PORTB
#define EPD_PORT PORTB
#define SPI_DDR DDRB
#define EPD_DDR DDRB
#define EPD_PINS PINB
#define SPI_ENABLE_BIT SPE
#define SPI_MASTER_MODE_BIT MSTR
#define SPI_RATE_BIT_0 SPR0
#define SPI_RATE_BIT_1 SPR1
#define SPI_RATE_2X_BIT SPI2X
#define SCREEN_WIDTH 128U
#define SCREEN_HEIGHT 296UL
#define F_CPU 8000000UL
#define __DELAY_BACKWARD_COMPATIBLE__

#define HIGH 1
#define LOW 0

#define BAUD 9600
#define UBRR_VAL 51






//#define SCREEN_DECIMATE
#define CHAR_MODE




#define CHAR_BUF_WIDTH 16
#define CHAR_BUF_HEIGHT 18
#define CHAR_BUF_LEN_BYTES 288
#define CHAR_WIDTH 8
#define CHAR_HEIGHT 16

// EPD SPI COMMANDS
#define DATA 1
#define COMMAND 0
#define EPD_DOC 0x1
#define EPD_BSSC 0xc
#define EPD_DSM 0x10
#define EPD_DEM 0x11
#define EPD_SWRST 0x12
#define EPD_WTEMP 0x1a
#define EPD_UPD 0x20
#define EPD_DUC1 0x21
#define EPD_DUC2 0x22
#define EPD_WRAM 0x24
#define EPD_WVCOM 0x2c
#define EPD_WLUT 0x32
#define EPD_SDLP 0x3a
#define EPD_SGLT 0x3b
#define EPD_BWCL 0x3c
#define EPD_SRXP 0x44
#define EPD_SRYP 0x45
#define EPD_SRXC 0x4e
#define EPD_SRYC 0x4f
#define EPD_NOP 0xff


#endif /* DEFINES_H_ */