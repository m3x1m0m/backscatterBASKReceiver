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
#include <iostream>
namespace backscatter {
namespace infrastructure {
namespace message {

class SampleMessage: public Message {
public:
	SampleMessage() :
			size(0), writeIndex(0), readIndex(0), sampleRate(0), sampless(0) {
	}
	SampleMessage(long sampleRate, size_t size);
	void addSample(unsigned int sample);
	bool hasSample(void) {
		return readIndex < writeIndex;
	}
	unsigned int nextSample(void);
	virtual ~SampleMessage();

	long getSampleRate() const {
		return sampleRate;
	}

	size_t getSize() const {
		return size;
	}

private:
	size_t size;
	size_t writeIndex;
	size_t readIndex;
	long sampleRate;
	unsigned int * sampless;
};

} /* namespace message */
} /* namespace infrastructure */
} /* namespace backscatter */

#endif /* INFRASTRUCTURE_MESSAGES_SAMPLEMESSAGE_H_ */
