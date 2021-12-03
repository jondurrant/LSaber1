/*
 * BladeStateAgent.cpp
 *
 *  Created on: 3 Dec 2021
 *      Author: jondurrant
 */

#include "BladeStateAgent.h"

#include "BladeRequest.h"
#include "SaberState.h"

BladeStateAgent::BladeStateAgent() {
	// TODO Auto-generated constructor stub

}

BladeStateAgent::~BladeStateAgent() {
	// TODO Auto-generated destructor stub
}

/***
 *  create the vtask, will get picked up by scheduler
 *
 *  */
void BladeStateAgent::start(UBaseType_t priority){
	xMessageBuffer = xMessageBufferCreate( STATE_MSG_BUF_LEN );
	if (xMessageBuffer == NULL){
		LogError( ("Create buf failed") );
		return;
	}
	if (xMessageBuffer != NULL){
		xTaskCreate(
			BladeStateAgent::vTask,
			"MQTTstate",
			512,
			( void * ) this,
			priority,
			&xHandle
		);
	}
}

/***
 * Internal function used by FreeRTOS to run the task
 * @param pvParameters
 */
void BladeStateAgent::vTask( void * pvParameters ){
	BladeStateAgent *task = (BladeStateAgent *) pvParameters;
	task->run();
}

/***
 * Internal function to run the task from within the object
 */
void BladeStateAgent::run(){

	BladeRequest req;
	SaberState *pSaberState = (SaberState *) pState;

	for (;;){
		if (xMessageBufferIsEmpty(xMessageBuffer) == pdTRUE){
			bool b = req.readFromQueue();
			if (b == true){
				if (req.getReq() == BladeOn){
					if (pSaberState->getOn() == false){
						pSaberState->setOn(true);
					}
				}
				if (req.getReq() == BladeOff){
					if (pSaberState->getOn() == true){
						pSaberState->setOn(false);
					}
				}
			}
			taskYIELD();
		} else {
			vTaskDelay( 20 );
			size_t size = xMessageBufferReceive(xMessageBuffer,
								xMsg, STATE_MAX_MSG_LEN, 0);
			if (size > 0){
				if (pState != NULL){
					processJson(xMsg);
				}
			}
		}
	}
}
