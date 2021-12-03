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

protected:
	static void vTask( void * pvParameters );

	virtual void run();


};

#endif /* BLADESTATEAGENT_H_ */
