/*
 * PacketDecoderListener.cpp
 *
 *  Created on: Mar 15, 2017
 *      Author: elmar
 */

#include "PacketDecoderListener.h"

namespace backscatter {
namespace infrastructure {
namespace listener {

#define PREAMBLE_LEN	4								//4 bytes, manchester encoded sending of 0x00 0x00
#define PREAMBLE_ONES	(PREAMBLE_LEN) * PREAMBLE_LEN	//4 ones per byte
#define PREAMBLE_ZEROS	(PREAMBLE_LEN) * PREAMBLE_LEN	//4 zeros per byte

#define PACKET_SYNC_LEN	4								//4 bytes manchester

PacketDecoderListener::PacketDecoderListener() :
		previousSample(0), preambleBit(0), preambleOnes(0), preambleZeros(0), baudrate(0) {
	// TODO Auto-generated constructor stub

}

void PacketDecoderListener::receiveMessage(Message* message) {
	SampleMessage * msg = (SampleMessage *) message;
	/*
	 int previousSample;
	 int preambleBit;
	 int preambleOnes;
	 int preamblezeros;
	 */
	while (msg->hasSample()) {
		unsigned int sample = msg->nextSample();
		switch (state) {
		case PREAMBLE:
			//10101010 10101010
			if (!previousSample && sample) {
				//rising edge
				preambleOnes++;
			} else if (previousSample && !sample) {
				//falling edge
				preambleZeros++;
				if (PREAMBLE_ZEROS == preambleZeros) {
					//finished with the preamble
					stop = std::chrono::high_resolution_clock::now();
					bitDuration = stop - start;
					preambleZeros = 0;
					preambleOnes = 0;
					//calculate baudrate
					state = PACKET_SYNC;
					start = std::chrono::high_resolution_clock::now();
				}
			}
			break;
		case PACKET_SYNC:
			sampleBuffer = (sampleBuffer << 1) | ((sample) ? 1 : 0);
			sampleBufferLen++;
			if (std::chrono::high_resolution_clock::now() - start >= bitDuration) {
				int temp = 0;
				for (int i = 0; i < sampleBufferLen; i++) {
					temp += (sampleBuffer & (1 << i)) ? 1 : -1;
				}
				sync <<= 1;
				sync |= (temp) ? 1 : 0;
				sampleBufferLen = 0;
				syncLen++;
				start = std::chrono::high_resolution_clock::now();
				if (syncLen >= PACKET_SYNC_LEN) {
					std::cout<<"Sync: " << sync<<std::endl;
					syncLen = 0;
					state = PACKET_SIZE;
				}
			}
			break;
		case PACKET_SIZE:

			break;
		case PACKET_DATA:
			break;
		case IDLE:
			if (sample && !previousSample) {
				state = PREAMBLE;
				start = std::chrono::high_resolution_clock::now();
				//save time
			}
			break;
		}
		previousSample = sample;
	}
}

int PacketDecoderListener::calculateBaudrate(SampleMessage * message) {
}

PacketDecoderListener::~PacketDecoderListener() {
	// TODO Auto-generated destructor stub
}

} /* namespace listener */
} /* namespace infrastructure */
} /* namespace backscatter */
