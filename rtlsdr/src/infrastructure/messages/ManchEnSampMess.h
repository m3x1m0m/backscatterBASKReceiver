//----------------------------------------------------------------------------------------------------------------------------
// Project:    	Backscatter BASK Receiver
// Name:		MachnEnSampMess.h
// Author:		Maximilian Stiefel
// Date:		20.03.2017
//
// Description:
//
//----------------------------------------------------------------------------------------------------------------------------

#ifndef INFRASTRUCTURE_MESSAGES_MANCHENSAMPMESS_H_
#define INFRASTRUCTURE_MESSAGES_MANCHENSAMPMESS_H_


#include <stdint.h>
#include "Message.h"

namespace backscatter {
namespace infrastructure {
namespace message {

class ManchEnSampMess: public Message {
public:
	ManchEnSampMess() :
			writeIndex(0), readIndex(0), sampleRate(0), length(0), samples(0)  {
	}
	ManchEnSampMess(unsigned int isampleRate, unsigned int ilength);
	void addSample(unsigned int sample) ;
	bool hasSample(void) {
			return readIndex < writeIndex;
		}
	unsigned int nextSample(void);
	virtual ~ManchEnSampMess();
	unsigned int getSize();
	unsigned int getSampleRate(void);

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


#endif /* INFRASTRUCTURE_MESSAGES_MANCHENSAMPMESS_H_ */
