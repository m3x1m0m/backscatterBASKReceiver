#ifndef SENSITIVE_HPP_
#define SENSITIVE_HPP_

#include <initializer_list>
#include <algorithm>
#include <vector>

namespace backscatter {
namespace infrastructure {
//! This class is used to tell whether a service should react upon a message or not
/*!
 * Every service has two instances of this class, one for the outgoing messages and one for the incoming messages.
 * The @ref Sensitive::isSensitiveFor function is called every time a message will be processed
 */
class Sensitive {

public:
	/**
	 * Only constructor for Sensitive. The parameter takes a list containing MsgType as argument.
	 * @param sensitivity list containing all the MsgType's its sensible for
	 */
	Sensitive(std::initializer_list<int> sensitivity) :
			sensitivity(sensitivity) {
		std::sort(this->sensitivity.begin(), this->sensitivity.end());
	}
	/**
	 * runs a binary search trough all the MsgType's its sensitive for and compares it with sensible
	 * @param sensible the MsgType to check against
	 * @return true if sensible is in the @ref sensitivity vector false otherwise
	 */
	bool isSensitiveFor(int sensible) {
		return std::binary_search(sensitivity.begin(), sensitivity.end(), sensible);
	}
private:
	std::vector<int> sensitivity; /**< A vector containing all the MsgType's this Sensitive is sensible for */
};

}
}
#endif /* SENSITIVE_HPP_ */
