//----------------------------------------------------------------------------------------------------------------------------
// Project:    	Backscatter BPSK Receiver
// Name:		DemodulatorSim.h
// Author:		Maximilian Stiefel
// Date:		27.03.2017
//
// Description:
//
//----------------------------------------------------------------------------------------------------------------------------

#ifndef DEMODULATORSIM_H_
#define DEMODULATORSIM_H_

//-------------------------------------Libraries-------------------------------------------------------------------------------
#include "infrastructure/messages/ManchEnSampMess.h"
#include "infrastructure/MessageBus.h"

//-------------------------------------Namespaces-------------------------------------------------------------------------------
namespace backscatter{
using namespace std;
using namespace backscatter::infrastructure;

//-------------------------------------Defines---------------------------------------------------------------------------------

#define MSG_SIZE 100*(2*1024) 											// 2 Bytes per sample (I,Q)
#define FRAME_SIZE 512 													// Frame size in bit
#define PAUSE_LENGTH_MS 300 											// Length of pause between packages in ms
#define BAUDRATE 1000
#define SAMP_RATE 25e3
#define TASK_PERIOD_MS 1000

#define SAMP_PER_BIT (unsigned int)(SAMP_RATE/BAUDRATE)
#define SAMP_PER_PAUSE (SAMP_RATE*(PAUSE_LENGTH_MS/1000.0))
#define BUFFER_LENGTH (unsigned int)(SAMP_PER_BIT*FRAME_SIZE + SAMP_PER_PAUSE)// Buffer length

//-------------------------------------CDemodulator----------------------------------------------------------------------------
class DemodulatorSim
{
public:
	DemodulatorSim(MessageBus* iBus, unsigned int inumMsg, unsigned int iIndex);
	~DemodulatorSim();
	void continuousReadout();
	void initializeBuffer();
	bool incRingBuf();
private:
	MessageBus *msgBus;
	unsigned int buffer[BUFFER_LENGTH];									// Buffer where the data which shall be sent is stored
	unsigned int bufIndex;
	unsigned int msgCnt;
	unsigned int numMsg;
};
} /* namespace backscatter */



#endif /* DEMODULATORSIM_H_ */
