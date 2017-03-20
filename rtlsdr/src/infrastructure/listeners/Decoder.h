/*
 * Decoder.h
 *
 *  Created on: Mar 15, 2017
 *      Author: elmar
 */

#ifndef DECODER_H_
#define DECODER_H_

#include "../listeners/Listener.h"
#include <chrono>
#include <stdint.h>

#include "../messages/PacketMess.h"
#include "../messages/ManchEnSampMess.h"

namespace backscatter {
namespace infrastructure {
namespace listener {

enum packet_state {
	PREAMBLE, PACKET_SYNC, PACKET_SIZE, PACKET_DATA, IDLE
};

class Decoder: public Listener {
public:
	Decoder();
	virtual void receiveMessage(Message * message);
	virtual ~Decoder();
private:
	uint8_t manToDec(uint16_t man);
	int calculateBaudrate(ManchEnSampMess * message);
	int8_t previousSample;
	uint64_t sampleBuffer;
	uint64_t sampleBufferLen;
	uint64_t sync;
	uint64_t syncLen;
	int64_t syncCounter;
	uint64_t size;
	uint64_t sizeLen;
	int64_t sizeCounter;
	uint64_t data;
	uint64_t dataLen;
	int64_t dataCounter;
	uint64_t dataAmount;
	uint64_t samplesBit;
	uint64_t bitCounter;
	uint64_t preambleCounter;
	int preambleOnes;
	int preambleZeros;
	int baudrate;
	enum packet_state state = IDLE;
	enum packet_state previousState = IDLE;
	message::PacketMessage * packetMessage;
	bool stop = false;
};

} /* namespace listener */
} /* namespace infrastructure */
} /* namespace backscatter */

#endif /* DECODER_H_ */
