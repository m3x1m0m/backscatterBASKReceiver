/*
 * ManchesterDecoder.h
 *
 *  Created on: Mar 13, 2017
 *      Author: elmar
 */

#ifndef MANCHESTERDECODER_H_
#define MANCHESTERDECODER_H_

#include "infrastructure/listeners/Listener.h"
#include "infrastructure/messages/Message.h"
#include "infrastructure/messages/SampleMessage.h"

namespace backscatter {
using namespace backscatter::infrastructure::listener;
using namespace backscatter::infrastructure::message;

namespace app {
class ManchesterDecoder: public Listener {
public:
	ManchesterDecoder();
	virtual void receiveMessage(Message * message) override;
	virtual ~ManchesterDecoder();
};

} /* namespace app */
} /* namespace backscatter */

#endif /* MANCHESTERDECODER_H_ */
