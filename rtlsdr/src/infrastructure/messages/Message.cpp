/*
 * Message.cpp
 *
 *  Created on: Mar 8, 2017
 *      Author: elmar
 */

#include "Message.h"
#include "MessageTypes.h"
#include <iostream>
#include <typeinfo>

namespace backscatter {
namespace infrastructure {
namespace message {
Message::Message() :
		type(-1) {
	// TODO Auto-generated constructor stub
}

int Message::getType(void) {
	std::cout<<MessageTypes::get().getType(this)<<std::endl;
	return MessageTypes::get().getType(this);
	//return std::type_index(typeid(*this));;
}

Message::~Message() {
	// TODO Auto-generated destructor stub
}
}
}
} /* namespace backscatter */
