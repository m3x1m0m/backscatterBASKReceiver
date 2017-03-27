//----------------------------------------------------------------------------------------------------------------------------
// Project:    	Backscatter BPSK Receiver
// Name:		DemodulatorSim.cpp
// Author:		Maximilian Stiefel
// Date:		27.03.2017
//
// Description:
//
//----------------------------------------------------------------------------------------------------------------------------

//-------------------------------------Libraries------------------------------------------------------------------------------
#include <iostream>
#include <chrono>
#include <thread>
#include "infrastructure/messages/ManchEnSampMess.h"
#include "DemodulatorSim.h"

//-------------------------------------Namespaces-----------------------------------------------------------------------------
namespace backscatter {
using namespace backscatter::infrastructure;
using namespace backscatter::infrastructure::message;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds

//-------------------------------------Constructor----------------------------------------------------------------------------
DemodulatorSim::DemodulatorSim(MessageBus* iBus, unsigned int inumMsg, unsigned int iIndex):
		msgBus(iBus), numMsg(inumMsg){
	// Variables

	// Action
	initializeBuffer();
	std::cout << "DemodulatorSim: Buffer length-" << BUFFER_LENGTH << std::endl;
	if( (iIndex <=100) && (iIndex >=0) )
		bufIndex = (iIndex/100)*BUFFER_LENGTH;
	else{
		std::cout << "DemodulatorSim: Spasti!" << std::endl;
		bufIndex = 0;
	}
}

//-------------------------------------initializeBuffer-----------------------------------------------------------------------
void DemodulatorSim::initializeBuffer(){
	//Variables
	bool what = true;
	unsigned int deb = 0;

	//Action
	for(unsigned int i = 0; i < BUFFER_LENGTH; i++){						// Write pause samples first everywhere
		buffer[i] = 0;
	}

	for(unsigned int i = 0; i < FRAME_SIZE; i++){							// Write simple message samples 1010101...
		for(unsigned int j = 0; j < SAMP_PER_BIT; j++){
			what ? buffer[i*SAMP_PER_BIT + j] = 1 : buffer[i*SAMP_PER_BIT + j] = 0;
			deb++;
		}
		what = !what;
	}
	std::cout << "DemodulatorSim: " ;
	for(unsigned int i = 0; i < BUFFER_LENGTH; i++){
			std::cout << buffer[i];
	}
	std::cout << std::endl ;
	std::cout << "Deb: " << deb << std::endl;
}
//-------------------------------------Destructor-----------------------------------------------------------------------------
DemodulatorSim::~DemodulatorSim() {

}
//-------------------------------------incRingBuffer--------------------------------------------------------------------------
bool DemodulatorSim::incRingBuf() {
	if(bufIndex == BUFFER_LENGTH-1){
		bufIndex = 0;
		return true;
	}
	else{
		bufIndex++;
		return false;
	}
}

//-------------------------------------continuousReadout----------------------------------------------------------------------
void DemodulatorSim::continuousReadout(){
	// Variables
	unsigned int len = MSG_SIZE;
	ManchEnSampMess *msg;
	unsigned int i = 0;

	// Action
	std::cout << "DemodulatorSim: Message size-" << len << " byte" << std::endl;
	std::cout << "DemodulatorSim: Number of Samples For Each Message-" << len/2 << " byte" << std::endl;
	std::cout << "DemodulatorSim: Starting to push stuff to the bus." << std::endl;
	while(msgCnt<numMsg){
		msg = new ManchEnSampMess(SAMP_RATE, len);
		i = 0;
		while (i < len){
			i++;
			msg->addSample(buffer[bufIndex]);
			incRingBuf();															// Increase bufIndex ring buffer style
		}
		msgBus->pushMessage(msg);
		msgCnt++;
		std::cout << "DemodulatorSim: Pushed samples to bus." << std::endl;
		std::cout << "DemodulatorSim: Nu. of 8 Bit vals-" << i << std::endl;
		sleep_until(system_clock::now() + milliseconds(TASK_PERIOD_MS));			// Periodic task
	}

}
} /* namespace backscatter */
