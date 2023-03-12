/*
 * IncFile1.h
 *
 * Created: 21/08/2019 11:23:28 AM
 *  Author: s4357807
 */ 


#ifndef SPI1_H_
#define SPI1_H_

#include <stdint.h>

class SPI {
	public:
	SPI(void);
	~SPI(void);
	
	static void spi_init(void);
	static void send_byte(uint8_t byte);
	
};

#endif /* SPI1_H_ */