/*
 * ManchesterDecoder.cpp
 *
 *  Created on: Mar 13, 2017
 *      Author: elmar
 */

#include "ManchesterDecoder.h"
#include "infrastructure/messages/MessageTypes.h"
#include "infrastructure/Sensitive.hpp"

namespace backscatter {
using namespace backscatter::infrastructure::message;
namespace app {

ManchesterDecoder::ManchesterDecoder() {
	SampleMessage msg;
	setSensitive(backscatter::infrastructure::Sensitive( { MessageTypes::get().getType(&msg) }));
}

void ManchesterDecoder::receiveMessage(Message* message) {
}

ManchesterDecoder::~ManchesterDecoder() {
	// TODO Auto-generated destructor stub
}

} /* namespace app */
} /* namespace backscatter */
