//----------------------------------------------------------------------------------------------------------------------------
// Project:    	Backscatter BPSK Receiver
// Name:		ProcessedSampMess.cpp
// Author:		Maximilian Stiefel
// Date:		16.03.2017
//
// Description:
//
//----------------------------------------------------------------------------------------------------------------------------

//-------------------------------------Libraries------------------------------------------------------------------------------
#include "ProcessedSampMess.h"
#include <iostream>
#include <typeinfo>

namespace backscatter {
namespace infrastructure {
namespace message {

//-------------------------------------Constructor----------------------------------------------------------------------------
ProcessedSampMess::ProcessedSampMess(unsigned int isampleRate, unsigned int ilength) :
		readIndex(0), writeIndex(0) {
	this->samples = new unsigned int[ilength];
	this->sampleRate = isampleRate;
	this->length = ilength;
}

//-------------------------------------addSample------------------------------------------------------------------------------
void ProcessedSampMess::addSample(unsigned int sample) {
	samples[writeIndex] = sample;
	writeIndex++;
}

//-------------------------------------removeSample---------------------------------------------------------------------------
uint8_t ProcessedSampMess::removeSample(void) {
	readIndex++;
	return samples[readIndex - 1];
}

//-------------------------------------getSize-------------------------------------------------------------------------------
unsigned int ProcessedSampMess::getSize(void) {
	return this->length;
}

//-------------------------------------Destructor-----------------------------------------------------------------------------
ProcessedSampMess::~ProcessedSampMess() {
	if (samples)
		delete[] samples;
}

} /* namespace message */
} /* namespace infrastructure */
} /* namespace backscatter */

