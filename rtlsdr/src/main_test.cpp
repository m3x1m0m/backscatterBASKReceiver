//----------------------------------------------------------------------------------------------------------------------------
// Project:    	Backscatter BPSK Receiver
// Name:		main.cpp
// Author:		Maximilian Stiefel
// Date:		15.03.2017
//
// Description:
//
//----------------------------------------------------------------------------------------------------------------------------

#include <iostream>
#include <thread>
#include "RTLSDR.h"
#include "./infrastructure/listeners/SchmittTrigger.h"
#include <stdint.h>
#include "infrastructure/messages/SampleMessage.h"
#include "PacketDecoderListener.h"
#include <chrono>
using namespace std;
using namespace backscatter;
enum SendState {
	PREAMBLE, SYNC, SIZE, DATA, IDLE
};
uint16_t preamble = 0b1010101010101010;
uint16_t sync = 0b0110100110010110;		//01101001 0x69
uint16_t size = 0b0101010101011010;		//00000011 0x03
uint16_t data[3] = { 0b0101010101010110, 0b0101010101011001, 0b0101010101011010 };
uint8_t dataIndex = 0;

int main2() {
	SendState state = SendState::PREAMBLE;
	using namespace backscatter::infrastructure;
	using namespace backscatter::infrastructure::listener;
	MessageBus *bus = new MessageBus();
	bus->addListener(new PacketDecoderListener());
	thread t1(&MessageBus::runLoop, bus);
	int bitIndex = 15;
	int bufferIndex = 0;
	auto start = std::chrono::system_clock::now();
	message::SampleMessage * msg = new message::SampleMessage(0, 10);
	int count = 0;
	while (true) {
		count++;
		switch (state) {
		case SendState::PREAMBLE:
			msg->addSample((preamble & (1 << bitIndex)) >> bitIndex);
			break;
		case SendState::SYNC:
			msg->addSample((sync & (1 << bitIndex)) >> bitIndex);
			break;
		case SendState::SIZE:
			msg->addSample((size & (1 << bitIndex)) >> bitIndex);
			break;
		case SendState::DATA:
			msg->addSample((data[dataIndex] & (1 << bitIndex)) >> bitIndex);
			break;
		}
		bufferIndex++;
		if (bufferIndex == 10) {
			bus->pushMessage(msg);
			msg = new SampleMessage(0, 10);
			bufferIndex = 0;
		}
		if (count == 4) {
			bitIndex--;
			if (bitIndex == -1) {
				if (state == SendState::SYNC)
					state = SendState::SIZE;
				else if (state == SendState::PREAMBLE)
					state = SendState::SYNC;
				else if (state == SendState::SIZE)
					state = SendState::DATA;
				else if (state == SendState::DATA) {
					dataIndex++;
					if (dataIndex == 3) {
						dataIndex = 0;
						state = SendState::IDLE;
					}
				}
				bitIndex = 15;
			}
			count = 0;
		}
	}
	t1.join();
	return 0;
}
