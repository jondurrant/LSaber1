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

void BladeSeqNewHope::on(BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length){
	xBladeSeq = BladeSeqTurnOn;
	xStartTime = to_ms_since_boot(get_absolute_time());
}

void BladeSeqNewHope::off(BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length){
	xBladeSeq = BladeSeqTurnOff;
	xStartTime = to_ms_since_boot(get_absolute_time());
}

void BladeSeqNewHope::tick(BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length){

	if ((xBladeSeq == BladeSeqTurnOn) || (xBladeSeq == BladeSeqTurnOff)){
		uint32_t sinceStart = to_ms_since_boot(get_absolute_time()) - xStartTime;

		if (sinceStart > NEWHOPEMS){
			if (xBladeSeq == BladeSeqTurnOff){
				bladeOff(length, c, strip, length);
				xBladeSeq = BladeSeqOff;
			} else {
				bladeOn(length, c, strip, length);
				xBladeSeq - BladeSeqOn;
			}
			lastStep = 0;
			strip->show();
		} else {
			uint32_t step =  sinceStart / (NEWHOPEMS / length);
			if (step > lastStep){
				if (xBladeSeq == BladeSeqTurnOff){
					bladeOff(step, c, strip, length);
				} else {
					bladeOn(step, c, strip, length);
				}
				lastStep = step;
				strip->show();
			}
		}
	}
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
	bladeOn(length - count, c, strip, length);
}
