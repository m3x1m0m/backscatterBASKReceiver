/*
 * EasyDecoder.cpp
 *
 *  Created on: Mar 25, 2017
 *      Author: maximilian
 */

#include "EasyDecoder.h"
#include "../messages/MessageTypes.h"
#include "../messages/ManchEnSampMess.h"
#include "../messages/PacketMess.h"
#include <ctime>

namespace backscatter {
namespace infrastructure {
namespace listener {

EasyDecoder::EasyDecoder(uint64_t sampleFreq) :
		sampleFreq(sampleFreq), expectedMsgSize(EXPECTED_MSG_SIZE), numOnes(0), bitCnt(0), sampCnt(0), state(NOT_IN_SYNC), silence(true){
	message::ManchEnSampMess msg;
	setSensitive(Sensitive( { message::MessageTypes::get().getType(&msg) }));
	samplesPerBit = (sampleFreq / 10) / BAUDRATE;
	bitThreshold = (samplesPerBit*0.6);
	std::cout << "Decoder: bitThreshold-" << bitThreshold << " samplesPerBit-" << samplesPerBit << std::endl;

}

EasyDecoder::~EasyDecoder() {
	// TODO Auto-generated destructor stub
}


void EasyDecoder::receiveMessage(Message* message) {
	ManchEnSampMess * msg = (ManchEnSampMess *) message;
	uint8_t currentBit = 0;
	uint8_t indicator = 1;

	while (msg->hasSample()) {
		currentBit = msg->nextSample();
		switch (state) {
		case NOT_IN_SYNC:												// The receiver might be turned on while a package is sent
			sampCnt++;
			if(currentBit)
				numOnes++;
			if(sampCnt == samplesPerBit){
				bitCnt++;
				if(numOnes >= bitThreshold)								// A one has been detected
					silence = false;
				sampCnt = 0;
				numOnes = 0;
			}
			if(bitCnt >= 5){
				bitCnt = 0;
				if(silence)
					state = IDLE;										// Silence for 5 bits -> no transmission ongoing
				else
					silence = true;										// New chance
			}
			break;
		case IDLE:
			sampCnt++;
			if(currentBit)
				numOnes++;
			if(sampCnt == samplesPerBit){
				if(numOnes >= bitThreshold)
					state = MESSAGE;									// Expect message now
				else
																		// Just noise
				sampCnt = 0;
				numOnes = 0;
			}
			break;
		case MESSAGE:
			sampCnt++;
			if(currentBit)												// Another sample indicates a one
				numOnes++;
			if(sampCnt == samplesPerBit){
				if(numOnes >= bitThreshold)
					recvData.push_back(1);								// Push a one to the received data vec
				else
					recvData.push_back(0);								// Push a zero to the received data vec
				numOnes = 0;
				sampCnt = 0;
			}
			if(recvData.size() >= expectedMsgSize){
				state = IDLE;											// One message has been sent
			}															// Back to idle
			break;
		}
	}
}

} /* namespace listener */
} /* namespace infrastructure */
} /* namespace backscatter */

