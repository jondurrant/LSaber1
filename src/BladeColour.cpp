/*
 * BladeColour.cpp
 *
 *  Created on: 4 Dec 2021
 *      Author: jondurrant
 */

#include "BladeColour.h"
#include <ColorUtils.h>
#include <stdio.h>

BladeColour::BladeColour() {
	// TODO Auto-generated constructor stub

}

BladeColour::~BladeColour() {
	// TODO Auto-generated destructor stub
}

uint8_t BladeColour::getBlue() const {
	return blue;
}

void BladeColour::setBlue(uint8_t blue) {
	this->blue = blue;
}

uint8_t BladeColour::getGreen() const {
	return green;
}

void BladeColour::setGreen(uint8_t green) {
	this->green = green;
}

uint8_t BladeColour::getRed() const {
	return red;
}

void BladeColour::setRed(uint8_t red) {
	this->red = red;
}

void BladeColour::set(uint8_t red, uint8_t green, uint8_t blue){
	setRed(red);
	setGreen(green);
	setBlue(blue);

}

void BladeColour::get(uint8_t &r, uint8_t &g, uint8_t &b,
			uint8_t led, uint8_t length){

	if (isBlack()){
		if (length > 0){
			double val = ((double)led) / ((double) length - 1);
			ColorUtils::valToRGB (val, r, g, b);
			//printf("colour %d/%d = %f (%d, %d, %d)\n", led, length , val, r, g, b);
			return;
		}

	}

	r = getRed();
	g = getGreen();
	b = getBlue();
}

bool BladeColour::isBlack(){
	if ((red == 0) && (green == 0) && (blue == 0)){
		return true;
	} else {
		return false;
	}
}
