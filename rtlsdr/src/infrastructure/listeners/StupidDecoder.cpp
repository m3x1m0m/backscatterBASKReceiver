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
	samplesPerBit = (sampleFreq / 10) / BAUDRATE;
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
			bitCount++;
			if (bitCount < 5 * samplesPerBit) {
				break;
			}
			bitCount = 1;
			state = MESSAGE;
			zeroCounter = 0;
			break;
		case MESSAGE:
			if (!sample) {
				totalZero++;
			} else {
				totalOne++;
			}
			if (prevSample != sample) {
				zeroCounter = 0;
				if (sample) {
					risingEdge++;
				} else {
					fallingEdge++;
				}
			} else if (!sample) {
				zeroCounter++;
				//std::cout << "Zero: " << (int)zeroCounter <<" samplesPerBit: " << (int)samplesPerBit<< std::endl;
				if (zeroCounter >= samplesPerBit * 20 && risingEdge > 80) {
					uint32_t averageError = 0;
					if (risingEdge > 10)
						risingEdges.push_back(risingEdge);
					for (uint32_t i : risingEdges) {
						averageError += i;
					}
					if (risingEdges.size())
						averageError /= risingEdges.size();
					printf("Rise: %d fall: %d ones %d zeros %d average: %d\n", risingEdge, fallingEdge, totalOne, (totalZero - samplesPerBit * 25), averageError);
					fallingEdge = 0;
					risingEdge = 0;
					totalOne = 0;
					totalZero = 0;
					state = IDLE;
				}
			}
			break;
		}
		prevSample = sample;
	}

}

} /* namespace listener */
} /* namespace infrastructure */
} /* namespace backscatter */
