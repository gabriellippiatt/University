/**
 *  @filename   :   epdif.cpp
 *  @brief      :   Implements EPD interface functions
 *                  Users have to implement all the functions in epdif.cpp
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     August 10 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "epdif.h"
#include "spi.h"

EpdIf::EpdIf() {
};

EpdIf::~EpdIf() {
};

void EpdIf::DigitalWrite(uint8_t pin, uint8_t value) {
    EPD_PORT &= ~(1 << pin);
	EPD_PORT |= (value << pin);
}

uint8_t EpdIf::DigitalRead(uint8_t pin) {
    return (EPD_PINS & (1 << pin)) >> pin;
}

void EpdIf::DelayMs(uint16_t delaytime) {
    _delay_ms(delaytime);
}

void EpdIf::SpiTransfer(uint8_t data) {
    SPI_PORT &= ~(1 << EPD_CS_PIN); //clear chip select and data/command bits;
    send_byte(data);
	SPI_PORT |= (1 << EPD_CS_PIN); // set the chip select bit high.
}

uint8_t EpdIf::IfInit(void) {
    EPD_DDR |= (1 << EPD_CS_PIN | 1 << EPD_DC_PIN | 1 << EPD_RESET_PIN);
    EPD_DDR &= ~(1 << EPD_BUSY_PIN);
    EPD_PORT &= ~(1 << EPD_BUSY_PIN);
	spi_init();
    
    return 0;
}

