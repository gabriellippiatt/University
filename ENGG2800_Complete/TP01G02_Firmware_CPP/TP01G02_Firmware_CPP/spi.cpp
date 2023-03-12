/*
 * spi.cpp
 *
 * Created: 21/08/2019 11:12:51 AM
 *  Author: s4357807
 */ 

#include "spi.h"
#include "definitions.h"

SPI::SPI() {
};

SPI::~SPI() {
};

void SPI::spi_init() {
	PWR_RED_REG &= ~(1 << SPI_PWR_RED_BIT);
	SPI_DDR |= (1 << MOSI_PIN | 1 << SCK_PIN);
	SPI_DDR &= ~(1 << MISO_PIN);
	SPI_CONTROL_REGISTER |= (1 << SPI_ENABLE_BIT) | (1 << SPI_MASTER_MODE_BIT) |
	(0 << SPI_RATE_BIT_0) | (0 << SPI_RATE_BIT_1);
	SPI_STATUS_REGISTER |= (0 << SPI_RATE_2X_BIT);
}

void SPI::send_byte(uint8_t byte) {
	
	asm volatile ("nop");
	asm volatile ("nop");
	SPI_DATA_REGISTER = byte;
	while (!(SPI_STATUS_REGISTER & (1 << SPI_FINISHED_BIT)));
	
}