/*
 * MessageBus.cpp
 *
 *  Created on: Mar 8, 2017
 *      Author: elmar
 */

#include "MessageBus.h"

namespace backscatter {
namespace infrastructure {
using namespace message;
MessageBus::MessageBus() :
		running(true) {
	// TODO Auto-generated constructor stub

}

void MessageBus::pushMessage(Message* message) {
	messageBuffer.push_back(message);
	for (std::vector<listener::Listener *>::iterator it = listeners.begin(); it != listeners.end(); ++it) {
		if ((*it)->isSensitive(message)) {
			(*it)->receiveMessage(message);
		}
	}
}

void MessageBus::addListener(listener::Listener* listener) {
	listeners.push_back(listener);
}

void MessageBus::stop(void) {
}

MessageBus::~MessageBus() {
	// TODO Auto-generated destructor stub
}

void MessageBus::runLoop(void) {
}

}
} /* namespace backscatter */
