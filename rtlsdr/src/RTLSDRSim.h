//----------------------------------------------------------------------------------------------------------------------------
// Project:    	Backscatter BPSK Receiver
// Name:		RTLSDRSim.h
// Author:		Maximilian Stiefel
// Date:		26.03.2017
//
// Description:
//
//----------------------------------------------------------------------------------------------------------------------------


#ifndef RTLSDRSIM_H_
#define RTLSDRSIM_H_

//-------------------------------------Libraries-------------------------------------------------------------------------------
#include "rtl-sdr.h"
#include "infrastructure/messages/RawSampMess.h"
#include "infrastructure/MessageBus.h"

//-------------------------------------Namespaces-------------------------------------------------------------------------------
namespace backscatter{
using namespace std;
using namespace backscatter::infrastructure;

//-------------------------------------Defines---------------------------------------------------------------------------------

#define MY_MSG_SIZE 10*(2*1024) 											// 2 Bytes per sample (I,Q)
#define MY_TEST_CASE "../samples/sim_test_1.csv"

//-------------------------------------CDemodulator----------------------------------------------------------------------------
class RTLSDRSim
{
public:
	RTLSDRSim(MessageBus* iBus, unsigned int isampRate);
	~RTLSDRSim();
	void continuousReadout(bool repeat, unsigned int zeroPadFact);
	void initializeBuffer();
	bool incRingBuf();
private:
	MessageBus *msgBus;
	uint8_t *buffer;															// Buffer where the data which shall be sent is stored
	unsigned int bufferLength;
	unsigned int sampRate;
	unsigned int bufIndex;
};
} /* namespace backscatter */



#endif /* RTLSDRSIM_H_ */
