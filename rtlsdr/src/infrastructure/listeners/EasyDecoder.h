/*
 * EasyDecoder.h
 *
 *  Created on: Mar 25, 2017
 *      Author: maximilian
 */

#ifndef INFRASTRUCTURE_LISTENERS_EASYDECODER_H_
#define INFRASTRUCTURE_LISTENERS_EASYDECODER_H_

#include "../listeners/Listener.h"
#include <chrono>
#include <stdint.h>

#include "../messages/PacketMess.h"
#include "../messages/ManchEnSampMess.h"

#define BAUDRATE 1000
#define EXPECTED_MSG_SIZE 512
#define NUMBER_OF_FRAMES 10
#define ONES_EXPECTED 256

namespace backscatter {
namespace infrastructure {
namespace listener {

class EasyDecoder: public Listener {
public:
	enum DecodeState {
		MESSAGE, IDLE, POSSIBLE_START, NOT_IN_SYNC
	};
	EasyDecoder(uint64_t sampleFreq);
	unsigned int incRing(unsigned int index, unsigned int size);
	virtual void receiveMessage(Message * message);
	virtual ~EasyDecoder();
private:
	unsigned int sampleFreq;
	unsigned int samplesPerBit;
	DecodeState state;
	unsigned int bitThreshold;
	unsigned int sampCnt;
	unsigned int bitCnt;
	unsigned int numOnes;
	unsigned int expectedMsgSize;
	std::vector<unsigned int> recvData;
	bool silence;

	uint32_t sampleCount = 0;
	int32_t sampleVal = 0;
	uint8_t prevSample = 0;
	uint32_t risingEdge = 0;
	uint32_t fallingEdge = 0;
	uint32_t bitCount = 0;
	uint32_t zeroCounter = 0;
	uint32_t totalZero = 0, totalOne = 0;
	std::vector<uint32_t> risingEdges;
};

} /* namespace listener */
} /* namespace infrastructure */
} /* namespace backscatter */


#endif /* INFRASTRUCTURE_LISTENERS_EASYDECODER_H_ */
