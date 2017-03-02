/*
 * Packet.cpp
 *
 *  Created on: Mar 1, 2017
 *      Author: elmar
 */

#include "Packet.h"

Packet::Packet(uint16_t* binString, size_t len) {
	manchesterDecode(binString, len);
}

size_t Packet::getByteSize() {
}

uint8_t Packet::getData(uint8_t buffer[]) {
}

Packet::~Packet() {
	// TODO Auto-generated destructor stub
}

void Packet::manchesterDecode(uint16_t * binString, size_t len) {
	size_t l = len * 8;

	for(int i = 0; i<len; i++){
		uint8_t temp = 0;
		for(int b = 16; b>0; b-=2){
			if(binString[b] && !binString[b-1]){
				temp++;
			}else if(binString[b] && !binString[b-1]){
				//zero
			} else{
				//end of packet?
			}
			temp<<=1;
		}
		byteData.push_back(temp);
	}
}
