//----------------------------------------------------------------------------------------------------------------------------
// Project:    	Backscatter BPSK Receiver
// Name:		RTLSDR.h
// Author:		Maximilian Stiefel
// Date:		15.03.2017
//
// Description:
//
//----------------------------------------------------------------------------------------------------------------------------


#ifndef RTLSDR_H_
#define RTLSDR_H_

//-------------------------------------Libraries-------------------------------------------------------------------------------
#include "rtl-sdr.h"
#include "infrastructure/messages/RawSampMess.h"
#include "infrastructure/MessageBus.h"

//-------------------------------------Namespaces-------------------------------------------------------------------------------
namespace backscatter{
using namespace std;
using namespace backscatter::infrastructure;

//-------------------------------------Defines---------------------------------------------------------------------------------

#define MY_BUFFER_LENGTH 100*(2*1024) 										// 2 Bytes per sample (I,Q)

//-------------------------------------CDemodulator----------------------------------------------------------------------------
class RTLSDR
{
public:
	RTLSDR(unsigned int samp_rate,unsigned int frequency,unsigned int gain, MessageBus* bus);
	void processCallback(unsigned char *buf, uint32_t len, void *ctx);
	~RTLSDR();
	void continuousReadout();
private:
	rtlsdr_dev_t *dev;
	MessageBus *msgBus;
	static void rtlsdrCallback(unsigned char * buf, uint32_t len, void * ctx);
	unsigned int samp_rate;
	unsigned int frequency;
	unsigned int gain;
};
} /* namespace backscatter */
#endif /* RTLSDR_H_ */
