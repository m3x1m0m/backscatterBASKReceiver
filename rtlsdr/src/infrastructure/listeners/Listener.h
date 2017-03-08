/*
 * Listener.h
 *
 *  Created on: Mar 8, 2017
 *      Author: elmar
 */

#ifndef INFRASTRUCTURE_LISTENERS_LISTENER_H_
#define INFRASTRUCTURE_LISTENERS_LISTENER_H_

#include "../Sensitive.hpp"
#include "../messages/Message.h"

namespace backscatter {
namespace infrastructure {
namespace listener {


class Listener {
public:
	Listener();
	void setSensitive(Sensitive sensitive);
	bool isSensitive(message::Message * message);
	virtual void receiveMessage(message::Message * message) = 0;
	virtual ~Listener();
private:
	Sensitive sensitive; /**< List containing all the incoming message id's the service is sensitive for*/
};

}
} /* namespace infrastructure */
} /* namespace backscatter */

#endif /* INFRASTRUCTURE_LISTENERS_LISTENER_H_ */
