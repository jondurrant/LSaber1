/*
 * BladeSeqCandle.h
 *
 *  Created on: 23 Dec 2021
 *      Author: jondurrant
 */

#ifndef BLADESEQCANDLE_H_
#define BLADESEQCANDLE_H_

#include "BladeSeqAnim.h"

#define CANDLE_FRACTION 20

class BladeSeqCandle : public BladeSeqAnim {
public:
	BladeSeqCandle();
	virtual ~BladeSeqCandle();

protected:

	virtual void bladeOn(uint8_t count, BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length);
	virtual void bladeOff(uint8_t count, BladeColour *c, PicoLed::PicoLedController *strip, uint8_t length);

};

#endif /* BLADESEQCANDLE_H_ */
