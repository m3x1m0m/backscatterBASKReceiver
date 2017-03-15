//----------------------------------------------------------------------------------------------------------------------------
// Project:    	Backscatter BPSK Receiver
// Name:		RawSampMess.cpp
// Author:		Maximilian Stiefel
// Date:		15.03.2017
//
// Description:
//
//----------------------------------------------------------------------------------------------------------------------------

//-------------------------------------Libraries------------------------------------------------------------------------------
#include "RawSampMess.h"
#include <iostream>
#include <typeinfo>

namespace backscatter {
namespace infrastructure {
namespace message {

//-------------------------------------Constructor----------------------------------------------------------------------------
RawSampMess::RawSampMess(unsigned int isampleRate, unsigned int ilength) :
		readIndex(0), writeIndex(0) {
	this->samples = new uint8_t[ilength];
	this->sampleRate = isampleRate;
	this->length = ilength;
}

//-------------------------------------addSample------------------------------------------------------------------------------
void RawSampMess::addSample(uint8_t sample) {
	samples[writeIndex] = sample;
	writeIndex++;
}

//-------------------------------------removeSample---------------------------------------------------------------------------
uint8_t RawSampMess::removeSample(void) {
	readIndex++;
	return samples[readIndex - 1];
}

//-------------------------------------getSize-------------------------------------------------------------------------------
unsigned int RawSampMess::getSize(void) {
	return this->length;
}

//-------------------------------------Destructor-----------------------------------------------------------------------------
RawSampMess::~RawSampMess() {
	if (samples)
		delete[] samples;
}

} /* namespace message */
} /* namespace infrastructure */
} /* namespace backscatter */

