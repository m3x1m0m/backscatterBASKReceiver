/*
 * MessageBus.cpp
 *
 *  Created on: Mar 8, 2017
 *      Author: elmar
 */

#include "MessageBus.h"
#include <iostream>

namespace backscatter {
namespace infrastructure {
using namespace message;
MessageBus::MessageBus() :
		running(false), stopSignal(false) {
	// TODO Auto-generated constructor stub

}

void MessageBus::pushMessage(message::Message * message) {
	messageQueue.push(message);
}

void MessageBus::addListener(listener::Listener* listener) {
	std::lock_guard < std::mutex > guard(listenersMutex);
	listeners.push_back(listener);
}

void MessageBus::stop(void) {
	stopSignal = true;
}

MessageBus::~MessageBus() {
	if (!stopSignal)
		stop();
	// TODO Auto-generated destructor stub
}

bool MessageBus::isRunning(void) {
	return running;
}

void MessageBus::runLoop(void) {
	running = true;
	while (!stopSignal) {
		message::Message * message = messageQueue.pop();
		if (stopSignal) {
			//If we stopped we should stop!
			break;
		}
		if (message == NULL) {
			std::cout << "message == null" << std::endl;
			break;
		} else {
			//std::cout << "Messages in queue: " << messageQueue.size() << std::endl;
			for (std::vector<listener::Listener *>::iterator it = listeners.begin(); it != listeners.end(); ++it) {
				if ((*it)->isSensitive(message)) {
					(*it)->receiveMessage(message);
				}
			}
			delete message;
		}
	}
	running = false;
}

}
} /* namespace backscatter */
