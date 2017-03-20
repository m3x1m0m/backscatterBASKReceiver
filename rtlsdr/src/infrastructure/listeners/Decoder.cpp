/*
 * Decoder.cpp
 *
 *  Created on: Mar 15, 2017
 *      Author: elmar
 */

#include "Decoder.h"
#include "../messages/MessageTypes.h"
#include "../messages/ManchEnSampMess.h"
#include "../messages/PacketMess.h"
#include <ctime>

namespace backscatter {
namespace infrastructure {
namespace listener {

#define PREAMBLE_LEN	2								//2 bytes, manchester encoded sending of 0x00 0x00
#define PREAMBLE_ONES	(PREAMBLE_LEN) * 4				//4 ones per byte
#define PREAMBLE_ZEROS	(PREAMBLE_LEN) * 4				//4 zeros per byte

#define PACKET_SYNC_LEN	2								//2 bytes manchester

Decoder::Decoder() :
		previousSample(0), sampleBuffer(0), sampleBufferLen(0), sync(0), syncLen(0), syncCounter(0), size(0), sizeLen(0), sizeCounter(0), data(0), dataLen(0), dataCounter(0), dataAmount(0), samplesBit(
				0), bitCounter(0), preambleOnes(0), preambleZeros(0), baudrate(0) {
	message::ManchEnSampMess msg;
	setSensitive(Sensitive( { message::MessageTypes::get().getType(&msg) }));
	// TODO Auto-generated constructor stub

}

void Decoder::receiveMessage(Message* message) {
	ManchEnSampMess * msg = (ManchEnSampMess *) message;
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
			if (samplesBit) {
				bitCounter++;
				if (bitCounter >= samplesBit) {
					state = PACKET_SYNC;
					bitCounter = 0;
				}
			}
			preambleCounter++;
			//10101010 10101010
			if (!previousSample && sample) {
				//rising edge
				preambleOnes++;
			} else if (previousSample && !sample) {
				//falling edge
				preambleZeros++;
				if (PREAMBLE_ZEROS == preambleZeros) {
					//finished with the preamble

					//calculate baudrate
					samplesBit = (preambleCounter) / ((PREAMBLE_LEN * 8) - 1);
					std::cout << "SamplesPerBit: " << samplesBit << std::endl;

					//reset
					bitCounter = 1;
					preambleZeros = 0;
					preambleOnes = 0;
				}
			}
			break;
		case PACKET_SYNC:
			syncCounter += (sample) ? 1 : -1;
			bitCounter++;
			if (bitCounter >= samplesBit) {
				sync = (sync << 1) | ((syncCounter > 0) ? 1 : 0);
				bitCounter = 0;
				syncCounter = 0;
				syncLen++;
				if (syncLen >= 16) {
					state = PACKET_SIZE;
					bitCounter = 0;
					syncLen = 0;
				}
			}
			break;
		case PACKET_SIZE:
			sizeCounter += (sample) ? 1 : -1;
			bitCounter++;
			if (bitCounter >= samplesBit) {
				size = (size << 1) | ((sizeCounter > 0) ? 1 : 0);
				bitCounter = 0;
				sizeCounter = 0;
				sizeLen++;
				if (sizeLen >= 16) {
					state = PACKET_DATA;
					sizeLen = 0;
					bitCounter = 0;
					packetMessage = new message::PacketMessage(manToDec(size), manToDec(sync));
				}
			}
			break;
		case PACKET_DATA:
			dataCounter += (sample) ? 1 : -1;
			bitCounter++;
			if (bitCounter >= samplesBit) {
				data = (data << 1) | ((dataCounter > 0) ? 1 : 0);
				bitCounter = 0;
				dataCounter = 0;
				dataLen++;
				if (dataLen >= 16) {
					bitCounter = 0;
					dataLen = 0;
					dataAmount++;
					packetMessage->addData(manToDec(data));
					data = 0;
					if (dataAmount == packetMessage->getSize()) {
						packetMessage->print();
						state = IDLE;
						//TODO push the message here
					}
				}
			}
			break;
		case IDLE:
			if (sample && !previousSample) {
				state = PREAMBLE;
				samplesBit = 0;
				preambleZeros = 0;
				preambleOnes = 1;
				//save time
			}
			break;
		}
		if (previousState != state) {
			std::cout << "State change: " << previousState << " -> " << state << std::endl;
		}
		previousSample = sample;
		previousState = state;
	}
}

uint8_t Decoder::manToDec(uint16_t man) {
	uint8_t byteVal = 0;
	for (int i = 8; i > 0; i--) {
		byteVal <<= 1;
		if ((man >> (i - 1) * 2 & 0x03) == 2) {
			byteVal |= 1;
		}
	}
	return byteVal;
}

int Decoder::calculateBaudrate(ManchEnSampMess * message) {
}

Decoder::~Decoder() {
	// TODO Auto-generated destructor stub
}

} /* namespace listener */
} /* namespace infrastructure */
} /* namespace backscatter */
