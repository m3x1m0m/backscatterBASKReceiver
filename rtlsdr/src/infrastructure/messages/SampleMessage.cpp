/*
 * SampleMessage.cpp
 *
 *  Created on: Mar 8, 2017
 *      Author: elmar
 */

#include "SampleMessage.h"
#include <iostream>
#include <typeinfo>
namespace backscatter {
namespace infrastructure {
namespace message {

SampleMessage::SampleMessage(long sampleRate, size_t size) :
		size(size), writeIndex(0), readIndex(0), sampleRate(0) {
	this->sampless = new unsigned int[size];
	this->sampleRate = sampleRate;
}

void SampleMessage::addSample(unsigned int sample) {
	sampless[writeIndex] = sample;
	writeIndex++;
}

unsigned int SampleMessage::nextSample(void) {
	readIndex++;
	return sampless[readIndex - 1];
}

SampleMessage::~SampleMessage() {
	if (sampless)
		delete[] sampless;
}

} /* namespace message */
} /* namespace infrastructure */
} /* namespace backscatter */
