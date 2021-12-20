/*
 * MQTTRouterSaber.cpp
 *
 *  Created on: 20 Dec 2021
 *      Author: jondurrant
 */

#include "MQTTRouterSaber.h"
#include <MQTTTopicHelper.h>

#define STATE_ON "{\"delta\": {\"on\": true}}"
#define STATE_OFF "{\"delta\": {\"on\": false}}"

MQTTRouterSaber::MQTTRouterSaber() {
	// NOP

}

MQTTRouterSaber::~MQTTRouterSaber() {
	if (pGroupTopicOn != NULL){
		vPortFree(pGroupTopicOn);
		pGroupTopicOn = NULL;
	}
	if (pGroupTopicOff != NULL){
		vPortFree(pGroupTopicOff);
		pGroupTopicOff = NULL;
	}
}

/***
 * Initialise the object give the Id and MQTT Interface
 * @param id = string ID of the Thing
 * @param mi = MQTT Interface
 */
void MQTTRouterSaber::init(const char * id, MQTTInterface *mi){
	MQTTRouterTwin::init(id, mi);

	if (pGroupTopicOn == NULL){
		pGroupTopicOn = (char *)pvPortMalloc(
				MQTTTopicHelper::lenGroupTopic(GROUP_NAME, ON_TOPIC)
				);
		if (pGroupTopicOn != NULL){
			MQTTTopicHelper::genGroupTopic(pGroupTopicOn, GROUP_NAME, ON_TOPIC);
		} else {
			LogError( ("Unable to allocate topic") );
		}
	}

	if (pGroupTopicOff == NULL){
		pGroupTopicOff = (char *)pvPortMalloc(
				MQTTTopicHelper::lenGroupTopic(GROUP_NAME, OFF_TOPIC)
				);
		if (pGroupTopicOff != NULL){
			MQTTTopicHelper::genGroupTopic(pGroupTopicOff, GROUP_NAME, OFF_TOPIC);
		} else {
			LogError( ("Unable to allocate topic") );
		}
	}
}

/***
 * Use the interface to setup all the subscriptions
 * @param interface
 */
void MQTTRouterSaber::subscribe(MQTTInterface *interface){
	MQTTRouterTwin::subscribe(interface);

	interface->subToTopic(pGroupTopicOn, 1);
	interface->subToTopic(pGroupTopicOff, 1);

}

/***
 * Route the message the appropriate part of the application
 * @param topic
 * @param topicLen
 * @param payload
 * @param payloadLen
 * @param interface
 */
void MQTTRouterSaber::route(const char *topic, size_t topicLen, const void * payload,
		size_t payloadLen, MQTTInterface *interface){
	MQTTRouterTwin::route(topic, topicLen, payload, payloadLen, interface);

	if (pTwin == NULL){
		LogError(("Twin not defined"));
		return;
	}

	if (strlen(pGroupTopicOn) == topicLen){
		if (memcmp(topic, pGroupTopicOn, topicLen)==0){
			LogDebug(("Blade ON"));
			pTwin->addMessage(STATE_ON, strlen(STATE_ON));
		}
	}
	if (strlen(pGroupTopicOff) == topicLen){
		if (memcmp(topic, pGroupTopicOff, topicLen)==0){
			LogDebug(("Blade OFF"));
			pTwin->addMessage(STATE_OFF,
					strlen(STATE_OFF)
					);
		}
	}
}

