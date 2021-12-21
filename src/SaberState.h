/*
 * SaberState.h
 *
 *  Created on: 28 Nov 2021
 *      Author: jondurrant
 */

#ifndef SABERSTATE_H_
#define SABERSTATE_H_

#include <MQTTConfig.h>
#include "StateTemp.h"
#include <stdbool.h>

#define SABER_RGB_SLOT 3
#define SABER_ON_SLOT  4
#define SABER_ID_SLOT 5

class SaberState : public StateTemp{
public:
	SaberState();
	SaberState(const SaberState &other);
	virtual ~SaberState();

	/***
	 * Get RGB as an array
	 * @return unsigned char[]
	 */
	const unsigned char* getRGB() const;

	/***
	 * Sets RGB based on array provided.
	 * Copies array so no need to allocate parameter
	 * @param newRGB - unsigned char[3]
	 */
	void setRGB(unsigned char* newRGB);

	/***
	 * Sets RGB based on three parameters
	 * @param r
	 * @param g
	 * @param b
	 */
	void setRGB(unsigned char r, unsigned char g, unsigned char b);

	/***
	 * Gets the On status of the light
	 * @return
	 */
	bool getOn() const;

	/***
	 * Sets the on status of the light
	 * @param b
	 */
	void setOn(bool b);

	/***
	 * Update state data from a json structure
	 * @param json
	 */
	virtual void updateFromJson(json_t const *json);

	/***
	 * Retrieve state of object in JSON format
	 * @param buf - buffer to write to
	 * @param len - length of buffer
	 * @return length of json or zero if we ran out of space
	 */
	virtual unsigned int state(char *buf, unsigned int len) ;

	/***
	 * Get Unit ID
	 * @return
	 */
	uint8_t getId() const ;

	/***
	 * Set Unit ID
	 * @param id
	 */
	void setId(uint8_t id = 0);

protected:
	/***
	 * Retrieve RGB in json format
	 * @param buf
	 * @param len
	 * @return
	 */
	char* jsonRGB(char *buf, unsigned int len);

	/***
	 * Retried On status in JSON format
	 * @param buf
	 * @param len
	 * @return
	 */
	char* jsonOn(char *buf, unsigned int len);

	/***
	 * Retried Id status in JSON format
	 * @param buf
	 * @param len
	 * @return
	 */
	char* jsonId(char *buf, unsigned int len);


private:
	//Light colour in RGB format, one bite per element
	unsigned char rgb[3] = {0, 0, 0};
	//Is light on
	bool on = false;

	uint8_t id = 0;

};

#endif /* SABERSTATE_H_ */
