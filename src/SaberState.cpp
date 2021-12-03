/*
 * SaberState.cpp
 *
 *  Created on: 28 Nov 2021
 *      Author: jondurrant
 */

#include "SaberState.h"

#include "hardware/adc.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "json-maker/json-maker.h"
#include "BladeRequest.h"

#define RGBSLOT 3
#define ONSLOT  4

/***
 * State will have 4 element two from StateTemperature and two added here
 */
SaberState::SaberState() {
	elements=5;

	jsonHelpers[RGBSLOT] = (StateFunc)&SaberState::jsonRGB;
	jsonHelpers[ONSLOT] = (StateFunc)&SaberState::jsonOn;

}

/***
 * Destructor
 */
SaberState::~SaberState() {
	// TODO Auto-generated destructor stub
}

/***
 * Copy constructor
 * @param other
 */
SaberState::SaberState(const SaberState &other) {
	on = other.getOn();
	memcpy(rgb, other.getRGB(), 3);
}

/***
 * Set On status of switch
 * @param bool
 */
void SaberState::setOn(bool b){
	on = b;
	BladeRequest req;
	if (on == true){
		req.setReq(BladeOn);
		LogDebug(("ON"));
	} else {
		req.setReq(BladeOff);
		LogDebug(("OFF"));
	}
	req.writeToQueue();

	setDirty(ONSLOT);
}

/***
 * Get on status of switch
 * @return
 */
bool SaberState::getOn() const{
	return on;
}

/***
 * get RGB status as array of unsigned chars
 * @return unsigned char[3]
 */
const unsigned char* SaberState::getRGB() const{
	return rgb;
}

/***
 * sets the RGB based on an array of RGN value
 * @param newRGB unsigned char [r, g, b]
 */
void SaberState::setRGB(unsigned char* newRGB){
	memcpy(rgb, newRGB, 3);
	BladeRequest req;
	req.setReq(BladeColour);
	req.setColour((uint8_t)newRGB[0],(uint8_t) newRGB[1],(uint8_t) newRGB[2]);
	req.writeToQueue();
	setDirty(RGBSLOT);
}

/***
 * Sets RGB based on three parameters
 * @param r
 * @param g
 * @param b
 */
void SaberState::setRGB(unsigned char r, unsigned char g, unsigned char b){
	rgb[0] = r;
	rgb[1] = g;
	rgb[2] = b;
	BladeRequest req;
	req.setReq(BladeColour);
	req.setColour((uint8_t)r,(uint8_t) g,(uint8_t) b);
	req.writeToQueue();
	setDirty(RGBSLOT);
}

/***
 * Retrieve on status in JSON format
 * @param buf
 * @param len
 * @return
 */
char* SaberState::jsonOn(char *buf, unsigned int len){
	char *p = buf;
    p = json_bool( p, "on", getOn(), &len);
    return p;
}

/***
 * Retrieve RGB valueu in JSON format
 * @param buf
 * @param len
 * @return
 */
char* SaberState::jsonRGB(char *buf, unsigned int len){
	char *p = buf;
    p = json_arrOpen( p, "rgb", &len);
    for (unsigned char i=0; i < 3; i++){
    	p = json_uint( p, NULL, getRGB()[i], &len );
    }
    p = json_arrClose( p, &len);
    return p;
}

/***
* Update state data from a json structure
* @param json
*/
void SaberState::updateFromJson(json_t const *json){
	StateTemp::updateFromJson(json);

	json_t const *jp;
	unsigned char newRGB[3]={0, 0, 0};
	bool rgbOK = true;

	jp = json_getProperty(json, "on");
	if (jp){
		if (JSON_BOOLEAN == json_getType(jp)){
			bool b = json_getBoolean(jp);
			setOn(b);
		}
	}

	jp = json_getProperty(json, "rgb");
	if (jp){
		if (JSON_ARRAY == json_getType(jp)){
			jp = json_getChild(jp);
			for (unsigned char i=0; i < 3; i++){
				if (jp){
					if (JSON_INTEGER == json_getType(jp)){
						int col = json_getInteger(jp);
						if ((col >= 0) && (col <= 0xFF)){
							newRGB[i] = (unsigned char) col;
						} else {
							rgbOK = false;
						}
					}
					else {
						rgbOK = false;
					}
				} else {
					rgbOK = false;
				}
				jp = json_getSibling(jp);
			}
			if (rgbOK){
				setRGB(newRGB);
			}
		}
	}

}

/***
 * Retrieve state of object in JSON format
 * @param buf - buffer to write to
 * @param len - length of buffer
 * @return length of json or zero if we ran out of space
 */
unsigned int SaberState::state(char *buf, unsigned int len){
	updateTemp();
	return StateTemp::state(buf, len);
}
