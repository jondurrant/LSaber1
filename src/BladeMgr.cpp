/*
 * BladeMgr.cpp
 *
 *  Created on: 3 Dec 2021
 *      Author: jondurrant
 */

#include "BladeMgr.h"
#include "BladeRequest.h"

#include "hardware/gpio.h"
#include <stdio.h>

BladeMgr * BladeMgr::pSelf = NULL;

BladeMgr::BladeMgr(uint8_t pirPin, uint8_t switchPin) {
	pSelf = this;
	xPirPin = pirPin;
	xSwitchPin = switchPin;

	//Set up PIR
	gpio_init(xPirPin);
	gpio_set_dir(xPirPin, GPIO_IN);
	gpio_set_irq_enabled_with_callback(xPirPin,
		GPIO_IRQ_EDGE_RISE,
		true,
		gpioCallback
	);

	//Set Switch
	gpio_init(xSwitchPin);
	gpio_set_dir(xSwitchPin, GPIO_IN);
	gpio_pull_up (xSwitchPin);
	gpio_set_irq_enabled_with_callback(xSwitchPin,
		GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL,
		true,
		gpioCallback
	);
}

BladeMgr::~BladeMgr() {
	// TODO Auto-generated destructor stub
}


void BladeMgr::loopForever(){
	BladeRequest req;
	uint32_t mesg;


	ledStrip.setBrightness(64);

	ledStrip.fill( PicoLed::RGB(0xff, 0, 0) );
	ledStrip.show();

	for(;;){
		if (req.readFromQueue()){
			switch(req.getReq()){
			case BladeColour: {
				setColour(req.getRed(),
						req.getGreen(),
						req.getBlue()
						);

				//printf("\nBladeMgr colour\n");

				break;
			}
			case BladeOn: {
				turnOn(true);
				break;
			}
			case BladeOff: {
				turnOff(true);
				break;
			}
			}
		}

		if (xTimeTurnedOn != 0){
			uint32_t timeOn = to_ms_since_boot(get_absolute_time()) - xTimeTurnedOn;
			if (timeOn > xOffTimeMS){
				turnOff(false);
				xTimeTurnedOn = 0;
			}
		}
	}


}

void BladeMgr::turnOn(bool remote){
	BladeRequest req;

	ledStrip.fill( PicoLed::RGB(red, green, blue));
	ledStrip.show();
	xTimeTurnedOn = to_ms_since_boot(get_absolute_time());

	if (!remote){
		req.setReq(BladeOn);
		req.writeToQueue();
	}
	//printf("\nBladeMgr on\n");
}

void BladeMgr::turnOff(bool remote){
	BladeRequest req;

	ledStrip.clear();
	ledStrip.show();
	xTimeTurnedOn = 0;

	if (!remote){
		req.setReq(BladeOff);
		req.writeToQueue();
	}
	//printf("\nBladeMgr off\n");
}



void BladeMgr::setColour(uint8_t r, uint8_t g, uint8_t b){
	red = r;
	green = g;
	blue = b;
}


void BladeMgr::gpioCallback (uint gpio, uint32_t events){
	BladeMgr::pSelf->handleGPIO(gpio, events);
}


void BladeMgr::handleGPIO (uint gpio, uint32_t events){
	if (gpio == xPirPin){
		if ((events & 0x08) > 0){
			turnOn(false);
		}
	}


	if (gpio == xSwitchPin){
		if ((events & GPIO_IRQ_EDGE_FALL) > 0){
			xSwitchTime = to_ms_since_boot(get_absolute_time ());
		}
		if ((events & GPIO_IRQ_EDGE_RISE) > 0){
			uint32_t t = to_ms_since_boot(get_absolute_time ()) - xSwitchTime;
			if ((t < 100) || (t > 5000)){
				return;
			} else if (t < 1000){
				handleShortPress();
			} else {
				handleLongPress();
			}
		}
	}
}


void BladeMgr::handleShortPress(){
	turnOn();
}

void BladeMgr::handleLongPress(){
	turnOn();
}

