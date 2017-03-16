//----------------------------------------------------------------------------------------------------------------------------
// Project:    	Backscatter BPSK Receiver
// Name:		ProcessedSampMess.cpp
// Author:		Maximilian Stiefel
// Date:		16.03.2017
//
// Description:
//
//----------------------------------------------------------------------------------------------------------------------------


#ifndef INFRASTRUCTURE_MESSAGES_PROCESSEDSAMPMESS_H_
#define INFRASTRUCTURE_MESSAGES_PROCESSEDSAMPMESS_H_

#include <stdint.h>
#include "Message.h"

namespace backscatter {
namespace infrastructure {
namespace message {

class ProcessedSampMess: public Message {
public:
	ProcessedSampMess() :
			writeIndex(0), readIndex(0), sampleRate(0), samples(0), length(0)  {
	}
	ProcessedSampMess(unsigned int isampleRate, unsigned int ilength);
	void addSample(unsigned int sample) ;
	uint8_t removeSample(void);
	virtual ~ProcessedSampMess();
	unsigned int getSize();
private:
	unsigned int writeIndex;
	unsigned int readIndex;
	unsigned int sampleRate;
	unsigned int length;
	unsigned int *samples;
};

} /* namespace message */
} /* namespace infrastructure */
} /* namespace backscatter */

#endif /* INFRASTRUCTURE_MESSAGES_PROCESSEDSAMPMESS_H_ */
