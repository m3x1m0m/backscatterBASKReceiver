/*
 * MessageTypes.h
 *
 *  Created on: Mar 8, 2017
 *      Author: elmar
 */

#ifndef INFRASTRUCTURE_MESSAGES_MESSAGETYPES_H_
#define INFRASTRUCTURE_MESSAGES_MESSAGETYPES_H_

#include <iostream>
#include <typeinfo>
#include <unordered_map>
#include <string>
#include <functional>
#include "Message.h"

namespace backscatter {
namespace infrastructure {
namespace message {

using TypeInfoRef = std::reference_wrapper<const std::type_info>;

struct Hasher {
	std::size_t operator()(TypeInfoRef code) const {
		return code.get().hash_code();
	}
};

struct EqualTo {
	bool operator()(TypeInfoRef lhs, TypeInfoRef rhs) const {
		return lhs.get() == rhs.get();
	}
};

class MessageTypes {
public:
	static MessageTypes& get() {
		static MessageTypes instance;
		return instance;
	}

	int getType(Message * message);
	// delete copy and move constructors and assign operators
	MessageTypes(MessageTypes const&) = delete;             // Copy construct
	MessageTypes(MessageTypes&&) = delete;                  // Move construct
	MessageTypes& operator=(MessageTypes const&) = delete;  // Copy assign
	MessageTypes& operator=(MessageTypes &&) = delete;      // Move assign
protected:
	MessageTypes();
	std::unordered_map<TypeInfoRef, int, Hasher, EqualTo> classToId;
	int curType;
	virtual ~MessageTypes();
};

} /* namespace message */
} /* namespace infrastructure */
} /* namespace backscatter */

#endif /* INFRASTRUCTURE_MESSAGES_MESSAGETYPES_H_ */
