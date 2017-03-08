/*
 * MessageBus.h
 *
 *  Created on: Mar 8, 2017
 *      Author: elmar
 */

#ifndef MESSAGEBUS_H_
#define MESSAGEBUS_H_
#include "messages/Message.h"
#include "messages/MessageTypes.h"
#include "listeners/Listener.h"
#include <vector>
namespace backscatter {
namespace infrastructure {
class MessageBus {
public:
	MessageBus();
	void pushMessage(message::Message * message);
	void addListener(listener::Listener * listener);
	virtual ~MessageBus();
private:
	std::vector<message::Message *> messageBuffer;
	std::vector<listener::Listener *> listeners;
};
}
} /* namespace backscatter */

#endif /* MESSAGEBUS_H_ */
