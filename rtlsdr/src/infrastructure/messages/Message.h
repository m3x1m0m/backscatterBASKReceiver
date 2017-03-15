/*
 * Message.h
 *
 *  Created on: Mar 8, 2017
 *      Author: elmar
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <iostream>
#include <typeinfo>
#include <unordered_map>
#include <string>
#include <functional>
#include <memory>

namespace backscatter {
namespace infrastructure {
namespace message {

class Message {
public:
	Message();
	int getType(void);
	virtual ~Message();
private:
	int type;
};
}
}
} /* namespace backscatter */

#endif /* MESSAGE_H_ */
