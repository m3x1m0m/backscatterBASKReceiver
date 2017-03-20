//----------------------------------------------------------------------------------------------------------------------------
// Project:    	Backscatter BASK Receiver
// Name:		MachnEnSampMess.cpp
// Author:		Maximilian Stiefel
// Date:		20.03.2017
//
// Description:
//
//----------------------------------------------------------------------------------------------------------------------------

//-------------------------------------Libraries------------------------------------------------------------------------------
#include "ManchEnSampMess.h"
#include <iostream>
#include <typeinfo>

namespace backscatter {
namespace infrastructure {
namespace message {

//-------------------------------------Constructor----------------------------------------------------------------------------
ManchEnSampMess::ManchEnSampMess(unsigned int isampleRate, unsigned int ilength) :
		writeIndex(0), readIndex(0) {
	this->samples = new unsigned int[ilength];
	this->sampleRate = isampleRate;
	this->length = ilength;
}

//-------------------------------------addSample------------------------------------------------------------------------------
void ManchEnSampMess::addSample(unsigned int sample) {
	samples[writeIndex] = sample;
	writeIndex++;
}

//-------------------------------------removeSample---------------------------------------------------------------------------
unsigned int ManchEnSampMess::nextSample(void) {
	readIndex++;
	return samples[readIndex - 1];
}

//-------------------------------------getSampleRate-------------------------------------------------------------------------
unsigned int ManchEnSampMess::getSampleRate(void) {
	return sampleRate;
}

//-------------------------------------getSize-------------------------------------------------------------------------------
unsigned int ManchEnSampMess::getSize(void) {
	return this->length;
}

//-------------------------------------Destructor-----------------------------------------------------------------------------
ManchEnSampMess::~ManchEnSampMess() {
	if (samples)
		delete[] samples;
}

} /* namespace message */
} /* namespace infrastructure */
} /* namespace backscatter */

