//-----------------------------------------------------------------------------------------------------------------------------
// Project:    	Backscatter BPSK Receiver
// Name:		EasyDecoder.cpp
// Author:		Maximilian Stiefel
// Date:		26.03.2017
//
// Description:
//
//-----------------------------------------------------------------------------------------------------------------------------

//-------------------------------------Libraries-------------------------------------------------------------------------------
#include "EasyDecoder.h"
#include "../messages/MessageTypes.h"
#include "../messages/ManchEnSampMess.h"
#include "../messages/PacketMess.h"
#include <ctime>

//-------------------------------------Namespaces-----------------------------------------------------------------------------
namespace backscatter {
namespace infrastructure {
namespace listener {

//-------------------------------------Constructor----------------------------------------------------------------------------
EasyDecoder::EasyDecoder(void) :
		sampleFreq(0), expectedMsgSize(EXPECTED_MSG_SIZE), numOnes(0), bitCnt(0), sampCnt(0), state(NOT_IN_SYNC),
		silence(true), samplesPerBit(0), bitThreshold(0), initialized(false), sumSamp(0), msgRecv(0){
	message::ManchEnSampMess msg;
	setSensitive(Sensitive( { message::MessageTypes::get().getType(&msg) }));
}

//-------------------------------------Destructor-----------------------------------------------------------------------------
EasyDecoder::~EasyDecoder() {
	// TODO Auto-generated destructor stub
}

//-------------------------------------receiveMessage-------------------------------------------------------------------------
void EasyDecoder::receiveMessage(Message* message) {
	ManchEnSampMess * msg = (ManchEnSampMess *) message;
	uint8_t currentBit = 0;
	unsigned int errors = 0;
	unsigned int expectation = 1;

	while (msg->hasSample()) {
		if(!initialized){												// This code is executed after each message
			samplesPerBit = msg->getSampleRate() / BAUDRATE;
			bitThreshold = (samplesPerBit*MY_BIT_THRESHOLD);
			std::cout << "Decoder: bitThreshold-" << bitThreshold << " samplesPerBit-" << samplesPerBit << std::endl;
			recvData.clear();											// Empty message vector
			sumSamp = 0;
			errors = 0;
			expectation = 1;
			initialized = true;
		}
		currentBit = msg->nextSample();									// get a single sample
		sumSamp++;
		switch (state) {
		case NOT_IN_SYNC:												// The receiver might be turned on while a package is sent
			//std::cout << "N ";
			sampCnt++;													// The decoder will be stuck in this state as long as there is no silence at least for 5 bits
			if(currentBit)
				numOnes++;
			if(sampCnt == samplesPerBit){
				bitCnt++;
				if(numOnes >= bitThreshold)								// A one has been detected
					silence = false;									// The silence has been disturbed
				sampCnt = 0;
				numOnes = 0;
			}
			if(bitCnt >= 5){
				bitCnt = 0;
				if(silence){
					state = IDLE;										// Silence for 5 bits -> no transmission ongoing
					std::cout << "Decoder: NOT_IN_SYNC->IDLE, sumSamp-" << sumSamp << std::endl ;
				}
				else
					silence = true;										// New chance
			}
			break;
		case IDLE:
			//std::cout << "I ";
			sampCnt++;
			if(currentBit)
				numOnes++;
			if(sampCnt == samplesPerBit){
				if(numOnes >= bitThreshold){							// A 1 has been detected
					if(expectation){									// Was it supposed to be detected?
						bitCnt++;
						expectation = 0;
					}
					else
						bitCnt = 0;
				}
				else{													// A 0 has been detected
					if(!expectation){									// Was it supposed to be detected?
						bitCnt++;
						expectation = 1;								// A 1 is the next thing to be expected
					}
					else
						bitCnt = 0;
				}
				sampCnt = 0;
				numOnes = 0;
			}
			if(bitCnt == PREAMBLE_LENGTH){								// Preamble has been detected
				state = MESSAGE;										// Message follow now
				std::cout << "Decoder: MESSAGE->IDLE, sumSamp-" << sumSamp << std::endl;
				for(unsigned int i = 0; i < PREAMBLE_LENGTH; i++)		// Add preamble to data
					recvData.push_back(!(i%2));
				bitCnt = 0;
			}
			break;
		case MESSAGE:
			sampCnt++;
			if(currentBit)												// Another sample indicates a one
				numOnes++;
			if(sampCnt == samplesPerBit){
				if(numOnes >= bitThreshold)
					recvData.push_back(1);								// Push a one to the received data vec
				else
					recvData.push_back(0);								// Push a zero to the received data vec
				numOnes = 0;
				sampCnt = 0;
			}
			if(recvData.size() >= expectedMsgSize){
				state = IDLE;											// One message has been sent
				msgRecv++;
				printData();
				errors = blingOracle();
				std::cout << "Decoder: BER is " << ((float)errors/(float)EXPECTED_MSG_SIZE)*100.0 << "%" << std::endl ;
				std::cout << "Decoder: MESSAGE->IDLE, sumSamp-" << sumSamp << std::endl ;
				std::cout << "Decoder: Message received-" << msgRecv << std::endl ;
				initialized = false;									// Calculate sampPerBit etc. again
			}
			break;
		}
	}
}

//-------------------------------------printMessage---------------------------------------------------------------------------
void EasyDecoder::printData(void) {
	std::cout << "Decoder: Received data-";
	for(unsigned int i=0; i<recvData.size(); i++)
		std::cout << recvData[i];
	std::cout << std::endl;
}
//-------------------------------------printMessage---------------------------------------------------------------------------
unsigned int EasyDecoder::blingOracle(void) {
	unsigned int expectedVal = 1;
	unsigned int errors = 0;

	for(unsigned int i = 0; i<recvData.size(); i++)
	{
		if(recvData[i] != expectedVal)
			errors++;
		expectedVal = !expectedVal;											// Detects 101010101 ...
	}
	return errors;
}

} /* namespace listener */
} /* namespace infrastructure */
} /* namespace backscatter */

