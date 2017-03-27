//----------------------------------------------------------------------------------------------------------------------------
// Project:    	Backscatter BPSK Receiver
// Name:		RTLSDRSim.cpp
// Author:		Maximilian Stiefel
// Date:		26.03.2017
//
// Description:
//
//----------------------------------------------------------------------------------------------------------------------------

//-------------------------------------Libraries------------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include <sstream>
#include <chrono>
#include <thread>
#ifndef _WIN32
#include <stdio.h>
#include <stdlib.h>
#endif
#include "RTLSDRSim.h"														// Own class definitions
#include "./infrastructure/messages/RawSampMess.h"

//-------------------------------------Namespaces-----------------------------------------------------------------------------
namespace backscatter {
using namespace backscatter::infrastructure;
using namespace backscatter::infrastructure::message;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds


//-------------------------------------Constructor----------------------------------------------------------------------------
RTLSDRSim::RTLSDRSim(MessageBus* iBus, unsigned int isampRate):
		msgBus(iBus), buffer(NULL), bufferLength(0), sampRate(isampRate), bufIndex(0){
	// Variables

	// Action
	initializeBuffer();
}

//-------------------------------------initializeBuffer-----------------------------------------------------------------------
void RTLSDRSim::initializeBuffer(){
	//Variables
	std::ifstream myfile(MY_TEST_CASE);
	std::string line;
	std::string oneSample;
	std::vector<float> buffer_tmp;

	//Action
	if (myfile.is_open()){
			while(std::getline(myfile,line)){										// Read one line
				std::stringstream lineStream(line);
				while(std::getline(lineStream,oneSample,',')){						// Read CSV values
					buffer_tmp.push_back(atof(oneSample.c_str()));					// Add one sample
				}
			}
			bufferLength = buffer_tmp.size();
			myfile.close();
			buffer = new uint8_t[bufferLength];
			for(unsigned int i=0; i<bufferLength;i++){								// Data has to be converted to the original ADC
				buffer[i]=(uint8_t)round((buffer_tmp[i]+127.5));					// Real, Imag, Real, Imag, ...
			}
			std::cout << "RTLSDRSim: Samples read-" << bufferLength/2 << std::endl;	// Two values belong to one sample
	} else
		std::cout << "Unable to open simulation file: " << MY_TEST_CASE << std::endl;
}
//-------------------------------------Destructor-----------------------------------------------------------------------------
RTLSDRSim::~RTLSDRSim() {
	if(buffer)
		delete [] buffer;
}
//-------------------------------------incRingBuffer--------------------------------------------------------------------------
bool RTLSDRSim::incRingBuf() {
	if(bufIndex == bufferLength-1){
		bufIndex = 0;
		return true;
	}
	else{
		bufIndex++;
		return false;
	}
}

//-------------------------------------continuousReadout----------------------------------------------------------------------
void RTLSDRSim::continuousReadout(bool repeat, unsigned int zeroPadFact){
	// Variables
	unsigned int len = MY_MSG_SIZE;
	RawSampMess *msg;
	unsigned int i = 0;
	bool wrapArround = false;

	// Action
	std::cout << "RTLSDRSim: Message size-" << len << " byte" << std::endl;
	std::cout << "RTLSDRSim: Number of Samples For Each Message-" << len/2 << " byte" << std::endl;
	std::cout << "RTLSDRSim: Zero padding factor-" << zeroPadFact << std::endl;
	std::cout << "RTLSDRSim: Starting to push stuff to the bus." << std::endl;
	while(1){
		msg = new RawSampMess(sampRate, len);
		i = 0;
		while (i < len){
			for(unsigned int j=0;j<zeroPadFact;j++){								// Zero Pad data and "interpolate"
				msg->addSample(buffer[bufIndex]);									// One needs to do that as the recorded data is downsampled
			}
			i+=zeroPadFact;
			wrapArround = incRingBuf();												// Increase bufIndex ring buffer style
			if(wrapArround && !repeat)												// If repeating is not desired -> stop here
				break;
		}
		if(wrapArround && !repeat){													// Just drop samples which do not lead to a complete message
			delete msg;
			std::cout << "RTLSDRSim: Nu. of 8 Bit vals-" << i << std::endl;
			std::cout << "RTLSDRSim: These samples are dropped." << std::endl;
			break;
		}
		msgBus->pushMessage(msg);
		std::cout << "RTLSDRSim: Pushed samples to bus." << std::endl;
		std::cout << "RTLSDRSim: Nu. of 8 Bit vals-" << i << std::endl;
		sleep_until(system_clock::now() + milliseconds(1000));								// Periodic task
	}

}
} /* namespace backscatter */
