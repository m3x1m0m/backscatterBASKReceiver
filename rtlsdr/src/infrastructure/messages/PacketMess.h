/*
 * Packet.h
 *
 *  Created on: Mar 16, 2017
 *      Author: elmar
 */

#ifndef PACKET_H_
#define PACKET_H_

#include "Message.h"
#include <stdio.h>
namespace backscatter {
namespace infrastructure {
namespace message {

class PacketMessage: public Message {
public:
	PacketMessage(uint8_t size, uint8_t sync);
	void addData(uint8_t data);
	virtual ~PacketMessage();

	uint8_t* getData() const {
		return data;
	}

	uint8_t getSize() const {
		return size;
	}

	uint8_t getSync() const {
		return sync;
	}

	uint8_t getWriteIndex() const {
		return writeIndex;
	}

	void setWriteIndex(uint8_t writeIndex) {
		this->writeIndex = writeIndex;
	}

	void print() {
		std::cout << "size[" << (int)size << "]sync(" << (int)sync << "){";
		for (int i = 0; i < writeIndex - 1; i++) {
			std::cout << (int)data[i] << ", ";
		}
		std::cout << (int)data[writeIndex - 1] << "}" << std::endl;
	}

private:
	uint8_t writeIndex;
	uint8_t size;
	uint8_t sync;
	uint8_t * data;

};
}
}
} /* namespace backscatter */

#endif /* PACKET_H_ */
