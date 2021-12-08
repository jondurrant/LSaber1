/*
 * BladeSeqNewHope.h
 *
 *  Created on: 8 Dec 2021
 *      Author: jondurrant
 */

#ifndef BLADESEQNEWHOPE_H_
#define BLADESEQNEWHOPE_H_

#include "BladeSeqInterface.h"

#define NEWHOPEMS 500

class BladeSeqNewHope : public BladeSeqInterface {
public:
	BladeSeqNewHope();
	virtual ~BladeSeqNewHope();

	virtual void on(BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length);

	virtual void off(BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length);

	virtual void tick(BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length);

private:

	BladeSeqEnum xBladeSeq = BladeSeqOff;
	uint32_t xStartTime = 0; //MS
	uint32_t lastStep = 0;

	void bladeOn(uint8_t count, BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length);
	void bladeOff(uint8_t count, BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length);
};

#endif /* BLADESEQNEWHOPE_H_ */
