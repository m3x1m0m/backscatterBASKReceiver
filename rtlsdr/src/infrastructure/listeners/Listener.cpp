/*
 * Listener.cpp
 *
 *  Created on: Mar 8, 2017
 *      Author: elmar
 */

#include "Listener.h"
#include "../messages/MessageTypes.h"
namespace backscatter {
namespace infrastructure {
namespace listener {
Listener::Listener() :
		sensitive(Sensitive( { })) {
	// TODO Auto-generated constructor stub
}

void Listener::setSensitive(Sensitive sensitive) {
	this->sensitive = sensitive;
}
bool Listener::isSensitive(message::Message* message) {
	if (sensitive.isSensitiveFor(message::MessageTypes::get().getType(message)))
		return true;
	return false;
}

Listener::~Listener() {
	// TODO Auto-generated destructor stub
}

}
} /* namespace infrastructure */
} /* namespace backscatter */

