/*
 * StupidDecoder.h
 *
 *  Created on: Mar 21, 2017
 *      Author: elmar
 */

#ifndef INFRASTRUCTURE_LISTENERS_STUPIDDECODER_H_
#define INFRASTRUCTURE_LISTENERS_STUPIDDECODER_H_
#include "../listeners/Listener.h"
#include <chrono>
#include <stdint.h>

#include "../messages/PacketMess.h"
#include "../messages/ManchEnSampMess.h"
namespace backscatter {
namespace infrastructure {
namespace listener {

class StupidDecoder: public Listener {
public:
	enum DecodeState {
		MESSAGE, IDLE
	};
	StupidDecoder();
	virtual void receiveMessage(Message * message);
	virtual ~StupidDecoder();
private:
	uint8_t prevSample = 0;
	uint32_t risingEdge = 0;
	uint32_t fallingEdge = 0;
	uint32_t bitCount = 0;
	DecodeState state = IDLE;
};

} /* namespace listener */
} /* namespace infrastructure */
} /* namespace backscatter */

#endif /* INFRASTRUCTURE_LISTENERS_STUPIDDECODER_H_ */
