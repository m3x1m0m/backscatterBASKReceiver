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

	int i = 0;
    int j = 0;

    while(i<len){
        if (binString[i]==0 && binString[i+1] == 1){
            byteData.push_back(0);
        }
        else if (binString[i]==1 && binString[i+1] == 0){
            byteData.push_back(1);
        }
        else printf("Error\n");
        i+=2;
	    j++;
    }

}
