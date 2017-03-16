/*
 * PacketDecoderListener.h
 *
 *  Created on: Mar 15, 2017
 *      Author: elmar
 */

#ifndef PACKETDECODERLISTENER_H_
#define PACKETDECODERLISTENER_H_

#include "infrastructure/listeners/Listener.h"
#include "infrastructure/messages/SampleMessage.h"
#include <chrono>
#include <stdint.h>

namespace backscatter {
namespace infrastructure {
namespace listener {

enum packet_state {
	PREAMBLE, PACKET_SYNC, PACKET_SIZE, PACKET_DATA, IDLE
};

class PacketDecoderListener: public Listener {
public:
	PacketDecoderListener();
	virtual void receiveMessage(Message * message);
	virtual ~PacketDecoderListener();
private:
	int calculateBaudrate(SampleMessage * message);
	std::chrono::high_resolution_clock::time_point start;
	std::chrono::high_resolution_clock::time_point stop;
	std::chrono::high_resolution_clock::time_point bitDuration;
	int8_t previousSample;
	uint64_t sampleBuffer;
	uint64_t sampleBufferLen;
	uint64_t sync;
	uint64_t syncLen;
	int preambleBit;
	int preambleOnes;
	int preambleZeros;
	int baudrate;
	enum packet_state state = IDLE;

};

} /* namespace listener */
} /* namespace infrastructure */
} /* namespace backscatter */

#endif /* PACKETDECODERLISTENER_H_ */
