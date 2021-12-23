/*
 * BladeSeqCandle.cpp
 *
 *  Created on: 23 Dec 2021
 *      Author: jondurrant
 */

#include "BladeSeqCandle.h"
#include <stdio.h>

BladeSeqCandle::BladeSeqCandle() {
	// TODO Auto-generated constructor stub

}

BladeSeqCandle::~BladeSeqCandle() {
	// TODO Auto-generated destructor stub
}

void BladeSeqCandle::bladeOn(uint8_t count, BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length){
	uint8_t r,g,b;

	uint8_t pixels = length / CANDLE_FRACTION;
	if (pixels < 0){
		pixels = 1;
	}

	strip->clear();
	int start = length - pixels;
	if (start < 0)
		start = 0;
	int end = count  + start;
	if (end > length )
		end = length;

	//printf("ON %d, %d->%d\n", count, start, end);

	for (uint i = start; i < end; i++){
		c->get(r, g, b, i, length);
		strip->setPixelColor(i, PicoLed::RGB(r, g, b));
	}
}

void BladeSeqCandle::bladeOff(uint8_t count, BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length){
	strip->clear();

	if (count < length){
		bladeOn(length - count, c, strip, length);
	}
}
