/*
 * MessageTypes.cpp
 *
 *  Created on: Mar 8, 2017
 *      Author: elmar
 */

#include "MessageTypes.h"
#include <iostream>
namespace backscatter {
namespace infrastructure {
namespace message {

MessageTypes::MessageTypes() : curType(0) {
	// TODO Auto-generated constructor stub

}

int MessageTypes::getType(Message* message) {
	if(classToId.find(typeid(*message)) == classToId.end()){
		classToId[typeid(*message)] = MessageTypes::curType++;
	}
	return classToId[typeid(*message)];
}

MessageTypes::~MessageTypes() {
	// TODO Auto-generated destructor stub
}

} /* namespace message */
} /* namespace infrastructure */
} /* namespace backscatter */
