/*
 * BladeMgr.h
 *
 *  Created on: 3 Dec 2021
 *      Author: jondurrant
 */

#ifndef BLADEMGR_H_
#define BLADEMGR_H_

#define LEDS_PIN 0
#define LEDS_LENGTH 10

#include <PicoLed.hpp>
#include "pico/stdlib.h"
#include <stdint.h>

class BladeMgr {
public:
	BladeMgr();
	virtual ~BladeMgr();


	void loopForever();

	void setColour(uint8_t r, uint8_t g, uint8_t b);

	void turnOn(bool remote = false);

	void turnOff(bool remote = false);


private:
	uint8_t red = 0;

	uint8_t green = 0;
	uint8_t blue = 0;

	uint32_t xTimeTurnedOn = 0;

	uint32_t xOffTimeMS = 5000; //MS

	PicoLed::PicoLedController ledStrip = PicoLed::addLeds<PicoLed::WS2812B>(pio0, 0, LEDS_PIN, LEDS_LENGTH, PicoLed::FORMAT_GRB);

};

#endif /* BLADEMGR_H_ */
