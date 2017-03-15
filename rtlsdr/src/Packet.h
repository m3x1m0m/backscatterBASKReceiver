/*
 * Packet.h
 *
 *  Created on: Mar 1, 2017
 *      Author: elmar
 */

#ifndef PACKET_H_
#define PACKET_H_
#include <stdint.h>
#include <vector>
#include <stdlib.h>

class Packet {
public:
	Packet(uint16_t * binString, size_t len);
	size_t getByteSize();
	uint8_t getData(uint8_t buffer[]);
	virtual ~Packet();
private:
	void manchesterDecode(uint16_t * binString, size_t len);
	std::vector<uint8_t> byteData;
};

#endif /* PACKET_H_ */
