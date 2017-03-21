/*
 * StupidDecoder.cpp
 *
 *  Created on: Mar 21, 2017
 *      Author: elmar
 */

#include "StupidDecoder.h"
#include "../messages/MessageTypes.h"
#include "../messages/ManchEnSampMess.h"
#include "../messages/PacketMess.h"
#include <ctime>

namespace backscatter {
namespace infrastructure {
namespace listener {
#define BAUDRATE 1000
StupidDecoder::StupidDecoder(uint64_t sampleFreq) :
		sampleFreq(sampleFreq) {
	message::ManchEnSampMess msg;
	setSensitive(Sensitive( { message::MessageTypes::get().getType(&msg) }));
	samplesPerBit = sampleFreq / BAUDRATE;
	// TODO Auto-generated constructor stub

}

StupidDecoder::~StupidDecoder() {
	// TODO Auto-generated destructor stub
}

void StupidDecoder::receiveMessage(Message* message) {
	ManchEnSampMess * msg = (ManchEnSampMess *) message;
	while (msg->hasSample()) {
		uint8_t sample = msg->nextSample();
		switch (state) {
		case IDLE:
			if (!sample) {
				break;
			}
			bitCount = 0;
			state = MESSAGE;
			zeroCounter = 0;
		case MESSAGE:
			if (prevSample != sample) {
				if (sample)
					risingEdge++;
				else
					fallingEdge++;
			} else if (!sample) {
				zeroCounter++;
				//std::cout << "Zero: " << (int)zeroCounter <<" samplesPerBit: " << (int)samplesPerBit<< std::endl;
				if (zeroCounter >= samplesPerBit * 10) {
					std::cout << "Rise: " << risingEdge << " fall: " << fallingEdge << std::endl;
					fallingEdge = 0;
					risingEdge = 0;
					state = IDLE;
				}
			}/*
			 sampleCount++;
			 sampleVal += (sample) ? 1 : -1;
			 if (sampleCount == samplesPerBit) {
			 std::cout<<"sampleVal:"<<sampleVal<<std::endl;
			 if (sampleVal > 0) {
			 risingEdge++;
			 zeroCounter = 0;
			 //we have a 1
			 } else if (sampleVal < 0) {
			 fallingEdge++;
			 zeroCounter++;
			 }
			 sampleCount = 0;
			 sampleVal = 0;
			 if (zeroCounter > 2) {
			 std::cout << "Rise: " << risingEdge << " fall: " << fallingEdge << std::endl;
			 fallingEdge=0;
			 risingEdge=0;
			 state = IDLE;
			 }

			 }*/
			break;
		}
		prevSample = sample;
	}

}

} /* namespace listener */
} /* namespace infrastructure */
} /* namespace backscatter */
