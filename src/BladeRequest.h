/*
 * BladeRequest.h
 *
 *  Created on: 28 Nov 2021
 *      Author: jondurrant
 */

#ifndef BLADEREQUEST_H_
#define BLADEREQUEST_H_

#include <stdlib.h>
#include "pico/stdlib.h"
#include <stdint.h>

enum BladeReqType {BladeOff, BladeOn, BladeColour};

class BladeRequest {
public:
	BladeRequest();
	virtual ~BladeRequest();

	void setColour(uint8_t red, uint8_t green, uint8_t blue);

	uint8_t getBlue() const;

	void setBlue(uint8_t blue);

	uint8_t getGreen() const;

	void setGreen(uint8_t green);

	uint8_t getRed()  const;

	void setRed(uint8_t red);

	BladeReqType getReq() const;

	void setReq(BladeReqType req) ;

	bool writeToQueue();

	bool readFromQueue();

	uint32_t get();

	void set(uint32_t mesg);

private:
	uint8_t red = 0;
	uint8_t green = 0;
	uint8_t blue = 0;

	BladeReqType req;

};

#endif /* BLADEREQUEST_H_ */
