//----------------------------------------------------------------------------------------------------------------------------
// Project:    	Backscatter BPSK Receiver
// Name:		RawSampMess.h
// Author:		Maximilian Stiefel
// Date:		15.03.2017
//
// Description:
//
//----------------------------------------------------------------------------------------------------------------------------

#ifndef INFRASTRUCTURE_MESSAGES_RAWSAMPMESS_H_
#define INFRASTRUCTURE_MESSAGES_RAWSAMPMESS_H_

#include <stdint.h>
#include "Message.h"

namespace backscatter {
namespace infrastructure {
namespace message {

class RawSampMess: public Message {
public:
	RawSampMess() :
			writeIndex(0), readIndex(0), sampleRate(0), samples(0), length(0)  {
	}
	RawSampMess(unsigned int isampleRate, unsigned int ilength);
	void addSample(uint8_t sample) ;
	uint8_t removeSample(void);
	virtual ~RawSampMess();
	unsigned int getSize();
private:
	unsigned int writeIndex;
	unsigned int readIndex;
	unsigned int sampleRate;
	unsigned int length;
	uint8_t* samples;
};

} /* namespace message */
} /* namespace infrastructure */
} /* namespace backscatter */

#endif /* INFRASTRUCTURE_MESSAGES_RAWSAMPMESS_H_ */
