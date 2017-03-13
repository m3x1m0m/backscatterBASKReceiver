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
#include <mutex>
#include "Queue.h"

namespace backscatter {
namespace infrastructure {
class MessageBus {
public:
	MessageBus();
	void pushMessage(std::shared_ptr<message::Message> message);
	void addListener(listener::Listener * listener);
	bool isRunning(void);
	void runLoop(void);
	void stop(void);
	virtual ~MessageBus();
private:
	std::mutex listenersMutex;
	Queue<std::shared_ptr<message::Message>> messageQueue; /**< Outgoing queue for data */
	std::vector<listener::Listener *> listeners;
	bool running;
	volatile bool stopSignal;
};
}
} /* namespace backscatter */

#endif /* MESSAGEBUS_H_ */
