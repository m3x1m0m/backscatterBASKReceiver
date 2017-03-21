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

StupidDecoder::StupidDecoder() {
	message::ManchEnSampMess msg;
	setSensitive(Sensitive( { message::MessageTypes::get().getType(&msg) }));
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
		case MESSAGE:
			if (sample == prevSample) {
				bitCount++;
				if (bitCount > 60) {
					//we are not sending anymore
					state = IDLE;
					std::cout << "\n Rising: " << (int) risingEdge << " Falling: " << (int) fallingEdge << std::endl;
					risingEdge = 0;
					fallingEdge = 0;
					break;
				}
			}
			if (sample != prevSample) {
				if (sample) {
					risingEdge++;
				} else {
					fallingEdge++;
				}
				std::cout << "{" << (int) sample << " " << (int) bitCount << "}";
				bitCount = 0;
			}
			break;
		}
		prevSample = sample;
	}

}

} /* namespace listener */
} /* namespace infrastructure */
} /* namespace backscatter */
