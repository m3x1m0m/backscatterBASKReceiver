/*
 * SampleMessage.h
 *
 *  Created on: Mar 8, 2017
 *      Author: elmar
 */

#ifndef INFRASTRUCTURE_MESSAGES_SAMPLEMESSAGE_H_
#define INFRASTRUCTURE_MESSAGES_SAMPLEMESSAGE_H_

#include <stdint.h>
#include "Message.h"

namespace backscatter {
namespace infrastructure {
namespace message {

class SampleMessage: public Message {
public:
	SampleMessage() :
			writeIndex(0), readIndex(0), sampleRate(0), sampless(0) {
	}
	SampleMessage(long sampleRate, size_t size);
	void addSample(unsigned int sample);
	unsigned int nextSample(void);
	virtual ~SampleMessage();
private:
	size_t writeIndex;
	size_t readIndex;
	long sampleRate;
	unsigned int * sampless;
};

} /* namespace message */
} /* namespace infrastructure */
} /* namespace backscatter */

#endif /* INFRASTRUCTURE_MESSAGES_SAMPLEMESSAGE_H_ */
