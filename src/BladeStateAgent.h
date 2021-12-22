/*
 * BladeStateAgent.h
 *
 *  Created on: 3 Dec 2021
 *      Author: jondurrant
 */

#ifndef BLADESTATEAGENT_H_
#define BLADESTATEAGENT_H_

#include <TwinTask.h>

class BladeStateAgent : public TwinTask {
public:
	BladeStateAgent();
	virtual ~BladeStateAgent();

	virtual void start(UBaseType_t priority);

	/***
	 * Notification of a change of a state item with the State object.
	 * @param dirtyCode - Representation of item changed within state. Used to pull back delta
	 */
	virtual void notifyState(uint16_t dirtyCode);

	void setTopics(char * onTopic, char * offTopic);

protected:
	static void vTask( void * pvParameters );

	virtual void run();


	void pubBladeState(bool on);

private:
	char * pOnTopic = NULL;
	char * pOffTopic = NULL;


};

#endif /* BLADESTATEAGENT_H_ */
