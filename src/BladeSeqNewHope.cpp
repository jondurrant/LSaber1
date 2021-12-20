/*
 * BladeSeqNewHope.cpp
 *
 *  Created on: 8 Dec 2021
 *      Author: jondurrant
 */

#include "BladeSeqNewHope.h"
#include "pico/stdlib.h"

BladeSeqNewHope::BladeSeqNewHope() {
	// TODO Auto-generated constructor stub

}

BladeSeqNewHope::~BladeSeqNewHope() {
	// TODO Auto-generated destructor stub
}

void BladeSeqNewHope::bladeOn(uint8_t count, BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length){
	uint8_t r,g,b;

	for (uint i = 0; i < count; i++){
		c->get(r, g, b, i, length);
		strip->setPixelColor(i, PicoLed::RGB(r, g, b));
	}
}

void BladeSeqNewHope::bladeOff(uint8_t count, BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length){
	strip->clear();
	if (count < length){
		bladeOn(length - count, c, strip, length);
	}
}

