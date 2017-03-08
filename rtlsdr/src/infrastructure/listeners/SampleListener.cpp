/*
 * SampleListener.cpp
 *
 *  Created on: Mar 8, 2017
 *      Author: elmar
 */

#include "SampleListener.h"
#include "../messages/SampleMessage.h"
#include "../messages/MessageTypes.h"
#include <iostream>

namespace backscatter {
namespace infrastructure {
namespace listener {
SampleListener::SampleListener() :
		Listener() {
	message::SampleMessage msg;
	setSensitive(Sensitive( { message::MessageTypes::get().getType(&msg) }));
	// TODO Auto-generated constructor stub

}

void SampleListener::receiveMessage(message::Message* message) {
	std::cout << "Received a message!" << std::endl;
}

SampleListener::~SampleListener() {
	// TODO Auto-generated destructor stub
}
}
} /* namespace infrastructure */
} /* namespace backscatter */
