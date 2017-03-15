/*
 * SampleListener.h
 *
 *  Created on: Mar 8, 2017
 *      Author: elmar
 */

#ifndef INFRASTRUCTURE_LISTENERS_SAMPLELISTENER_H_
#define INFRASTRUCTURE_LISTENERS_SAMPLELISTENER_H_
#include "Listener.h"
namespace backscatter {
namespace infrastructure {
namespace listener{
class SampleListener: public Listener {
public:
	SampleListener();
	virtual void receiveMessage(message::Message * message) override;
	virtual ~SampleListener();
};
}
} /* namespace infrastructure */
} /* namespace backscatter */

#endif /* INFRASTRUCTURE_LISTENERS_SAMPLELISTENER_H_ */
