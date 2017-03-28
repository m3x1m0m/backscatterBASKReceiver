//-----------------------------------------------------------------------------------------------------------------------------
// Project:    	Backscatter BPSK Receiver
// Name:		EasyDecoder.h
// Author:		Maximilian Stiefel
// Date:		26.03.2017
//
// Description:
//
//-----------------------------------------------------------------------------------------------------------------------------

#ifndef INFRASTRUCTURE_LISTENERS_EASYDECODER_H_
#define INFRASTRUCTURE_LISTENERS_EASYDECODER_H_

//-------------------------------------Libraries-------------------------------------------------------------------------------
#include "../listeners/Listener.h"
#include <chrono>
#include <stdint.h>

#include "../messages/PacketMess.h"
#include "../messages/ManchEnSampMess.h"

//-------------------------------------Defines---------------------------------------------------------------------------------
#define BAUDRATE 1000
#define EXPECTED_MSG_SIZE 512
#define MY_BIT_THRESHOLD 0.4
#define NUMBER_OF_FRAMES 10
#define ONES_EXPECTED 256
#define PREAMBLE_LENGTH 4								// Preamble must always look like 10101010...

//-------------------------------------Namespaces------------------------------------------------------------------------------
namespace backscatter {
namespace infrastructure {
namespace listener {

//-------------------------------------CDemodulator----------------------------------------------------------------------------
class EasyDecoder: public Listener {
public:
	enum DecodeState {
		MESSAGE, IDLE, NOT_IN_SYNC
	};
	EasyDecoder(void);
	unsigned int incRing(unsigned int index, unsigned int size);
	void printData(void);
	virtual void receiveMessage(Message * message);
	unsigned int blingOracle(void);
	virtual ~EasyDecoder();
private:
	unsigned int sampleFreq;
	unsigned int samplesPerBit;
	DecodeState state;
	unsigned int bitThreshold;
	unsigned int sampCnt;
	unsigned int bitCnt;
	unsigned int numOnes;
	unsigned int expectedMsgSize;
	std::vector<unsigned int> recvData;
	bool silence;
	bool initialized;
	unsigned int sumSamp;
	unsigned int msgRecv;
};

} /* namespace listener */
} /* namespace infrastructure */
} /* namespace backscatter */


#endif /* INFRASTRUCTURE_LISTENERS_EASYDECODER_H_ */
