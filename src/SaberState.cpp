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



/***
 * State will have 4 element two from StateTemperature and two added here
 */
SaberState::SaberState() {
	elements=11;

	jsonHelpers[SABER_DAY_RGB_SLOT] = (StateFunc)&SaberState::jsonDayRGB;
	jsonHelpers[SABER_NIGHT_RGB_SLOT] = (StateFunc)&SaberState::jsonNightRGB;
	jsonHelpers[SABER_ON_SLOT] = (StateFunc)&SaberState::jsonOn;
	jsonHelpers[SABER_ID_SLOT] = (StateFunc)&SaberState::jsonId;
	jsonHelpers[SABER_DAY_START_SLOT] = (StateFunc)&SaberState::jsonDayStart;
	jsonHelpers[SABER_DAY_END_SLOT] = (StateFunc)&SaberState::jsonDayEnd;
	jsonHelpers[SABER_DAY_SEQ_SLOT] = (StateFunc)&SaberState::jsonDaySeq;
	jsonHelpers[SABER_NIGHT_SEQ_SLOT] = (StateFunc)&SaberState::jsonNightSeq;
}

/***
 * Destructor
 */
SaberState::~SaberState() {
	// TODO Auto-generated destructor stub
}


/***
 * Set On status of switch
 * @param bool
 */
void SaberState::setOn(bool b){
	xOn = b;
	BladeRequest req;
	if (xOn == true){
		req.setReq(BladeOn);
		LogDebug(("ON"));
	} else {
		req.setReq(BladeOff);
		LogDebug(("OFF"));
	}
	req.writeToQueue();

	setDirty(SABER_ON_SLOT);
}

/***
 * Get on status of switch
 * @return
 */
bool SaberState::getOn() const{
	return xOn;
}

/***
 * get RGB status as array of unsigned chars
 * @return uint8_t[3]
 */
const uint8_t * SaberState::getDayRGB() const{
	return xDayRGB;
}

/***
 * get RGB status as array of unsigned chars
 * @return uint8_t[3]
 */
const uint8_t * SaberState::getNightRGB() const{
	return xNightRGB;
}

/***
 * sets the RGB based on an array of RGN value
 * @param newRGB uint8_t [r, g, b]
 */
void SaberState::setDayRGB(uint8_t* newRGB){
	memcpy(xDayRGB, newRGB, 3);
	BladeRequest req;
	req.setReq(BladeDRGB);
	req.setColour((uint8_t)newRGB[0],(uint8_t) newRGB[1],(uint8_t) newRGB[2]);
	req.writeToQueue();
	setDirty(SABER_DAY_RGB_SLOT);
}

/***
 * sets the RGB based on an array of RGN value
 * @param newRGB uint8_t [r, g, b]
 */
void SaberState::setNightRGB(uint8_t* newRGB){
	memcpy(xNightRGB, newRGB, 3);
	BladeRequest req;
	req.setReq(BladeNRGB);
	req.setColour((uint8_t)newRGB[0],(uint8_t) newRGB[1],(uint8_t) newRGB[2]);
	req.writeToQueue();
	setDirty(SABER_DAY_RGB_SLOT);
}

/***
 * Sets RGB based on three parameters
 * @param r
 * @param g
 * @param b
 */
void SaberState::setDayRGB( uint8_t r,  uint8_t g, uint8_t  b){
	xDayRGB[0] = r;
	xDayRGB[1] = g;
	xDayRGB[2] = b;
	BladeRequest req;
	req.setReq(BladeDRGB);
	req.setColour((uint8_t)r,(uint8_t) g,(uint8_t) b);
	req.writeToQueue();
	setDirty(SABER_DAY_RGB_SLOT);
}

/***
 * Sets RGB based on three parameters
 * @param r
 * @param g
 * @param b
 */
void SaberState::setNightRGB( uint8_t r,  uint8_t g, uint8_t  b){
	xDayRGB[0] = r;
	xDayRGB[1] = g;
	xDayRGB[2] = b;
	BladeRequest req;
	req.setReq(BladeNRGB);
	req.setColour((uint8_t)r,(uint8_t) g,(uint8_t) b);
	req.writeToQueue();
	setDirty(SABER_DAY_RGB_SLOT);
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
char* SaberState::jsonDayRGB(char *buf, unsigned int len){
	char *p = buf;
    p = json_arrOpen( p, "drgb", &len);
    for (unsigned char i=0; i < 3; i++){
    	p = json_uint( p, NULL, getDayRGB()[i], &len );
    }
    p = json_arrClose( p, &len);
    return p;
}

/***
 * Retrieve RGB valueu in JSON format
 * @param buf
 * @param len
 * @return
 */
char* SaberState::jsonNightRGB(char *buf, unsigned int len){
	char *p = buf;
    p = json_arrOpen( p, "nrgb", &len);
    for (unsigned char i=0; i < 3; i++){
    	p = json_uint( p, NULL, getNightRGB()[i], &len );
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

	jp = json_getProperty(json, "drgb");
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
				setDayRGB(newRGB);
			}
		}
	}

	jp = json_getProperty(json, "nrgb");
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
				setNightRGB(newRGB);
			}
		}
	}

	jp = json_getProperty(json, "id");
	if (jp){
		if (JSON_INTEGER == json_getType(jp)){
			setId(json_getInteger(jp));
		}
	}

	jp = json_getProperty(json, "days");
	if (jp){
		if (JSON_INTEGER == json_getType(jp)){
			setDayStart(json_getInteger(jp));
		}
	}

	jp = json_getProperty(json, "daye");
	if (jp){
		if (JSON_INTEGER == json_getType(jp)){
			setDayEnd(json_getInteger(jp));
		}
	}

	jp = json_getProperty(json, "dseq");
	if (jp){
		if (JSON_INTEGER == json_getType(jp)){
			setDaySeq(json_getInteger(jp));
		}
	}

	jp = json_getProperty(json, "nseq");
	if (jp){
		if (JSON_INTEGER == json_getType(jp)){
			setNightSeq(json_getInteger(jp));
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

uint8_t SaberState::getId() const {
	return xId;
}

void SaberState::setId(uint8_t id) {
	xId = id;
	setDirty(SABER_ON_SLOT);
}

/***
 * Retried Id status in JSON format
 * @param buf
 * @param len
 * @return
 */
char* SaberState::jsonId(char *buf, unsigned int len){
	char *p = buf;
	p = json_uint( p, "id", getId(), &len);
	return p;
}

/***
 * Returns hour at which Day Ends
 * @return
 */
uint8_t SaberState::getDayEnd() const {
	return xDayEnd;
}

/***
 * Sets hour at which day ends
 * @param DayEnd
 */
void SaberState::setDayEnd(uint8_t xDayEnd) {
	this->xDayEnd = xDayEnd;
	setDirty(SABER_DAY_END_SLOT);
}

/***
 * Get Start Day Hour
 * @return start hour
 */
uint8_t SaberState::getDayStart() const {
	return xDayStart;
}

/***
 * Sets Day start hour
 * @param xDayStart
 */
void SaberState::setDayStart(uint8_t xDayStart) {
	this->xDayStart = xDayStart;
	setDirty(SABER_DAY_START_SLOT);
}

/***
 * Retried Day Start Hour status in JSON format
 * @param buf
 * @param len
 * @return
 */
char* SaberState::jsonDayStart(char *buf, unsigned int len){
	char *p = buf;
	p = json_uint( p, "daye", getDayStart(), &len);
	return p;
}

/***
 * Retried Day End Hour status in JSON format
 * @param buf
 * @param len
 * @return
 */
char* SaberState::jsonDayEnd(char *buf, unsigned int len){
	char *p = buf;
	p = json_uint( p, "days", getDayEnd(), &len);
	return p;
}

/***
 * Get the sequence for the blade
 * int number of the sequence.
 * @return
 */
uint8_t SaberState::getDaySeq() const {
	return xDaySeq;
}

/***
 * Set the sequence number for the blade
 * @param xSeq
 */
void SaberState::setDaySeq(uint8_t xSeq) {
	this->xDaySeq = xSeq;
	setDirty(SABER_DAY_SEQ_SLOT);
}

/***
 * Retried Seq number in JSON format
 * @param buf
 * @param len
 * @return
 */
char* SaberState::jsonDaySeq(char *buf, unsigned int len){
	char *p = buf;
	p = json_uint( p, "dseq", getDaySeq(), &len);
	return p;
}

/***
 * Get the sequence for the blade
 * int number of the sequence.
 * @return
 */
uint8_t SaberState::getNightSeq() const {
	return xNightSeq;
}

/***
 * Set the sequence number for the blade
 * @param xSeq
 */
void SaberState::setNightSeq(uint8_t xSeq) {
	this->xNightSeq = xSeq;
	setDirty(SABER_NIGHT_SEQ_SLOT);
}

/***
 * Retried Seq number in JSON format
 * @param buf
 * @param len
 * @return
 */
char* SaberState::jsonNightSeq(char *buf, unsigned int len){
	char *p = buf;
	p = json_uint( p, "nseq", getNightSeq(), &len);
	return p;
}


