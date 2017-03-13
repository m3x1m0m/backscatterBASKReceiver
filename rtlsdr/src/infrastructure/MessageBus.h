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
#include <memory>
namespace backscatter {
namespace infrastructure {
class MessageBus {
public:
	MessageBus();
	void pushMessage(message::Message * message);
	void addListener(listener::Listener * listener);
	void stop(void);
	virtual ~MessageBus();
private:
	std::vector<std::unique_ptr<message::Message *>> messageBuffer;
	std::vector<listener::Listener *> listeners;
	void runLoop(void);
	bool running;
};
}
} /* namespace backscatter */

#endif /* MESSAGEBUS_H_ */
