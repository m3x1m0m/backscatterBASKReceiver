/*
 * Packet.cpp
 *
 *  Created on: Mar 16, 2017
 *      Author: elmar
 */

#include "PacketMess.h"

namespace backscatter {
namespace infrastructure {
namespace message {

PacketMessage::PacketMessage(uint8_t size, uint8_t sync) :
		writeIndex(0), sync(sync), size(size) {
	data = new uint8_t[size];
	// TODO Auto-generated constructor stub

}

void PacketMessage::addData(uint8_t data) {
	this->data[writeIndex] = data;
	writeIndex++;
}

PacketMessage::~PacketMessage() {
	delete[] data;
	// TODO Auto-generated destructor stub
}

} /* namespace listener */
} /* namespace infrastructure */
} /* namespace backscatter */
