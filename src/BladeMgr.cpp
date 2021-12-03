/*
 * BladeMgr.cpp
 *
 *  Created on: 3 Dec 2021
 *      Author: jondurrant
 */

#include "BladeMgr.h"
#include "BladeRequest.h"

#include <stdio.h>

BladeMgr::BladeMgr() {
	// TODO Auto-generated constructor stub

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
