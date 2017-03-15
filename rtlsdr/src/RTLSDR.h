//----------------------------------------------------------------------------------------------------------------------------
// Project:    	Backscatter BPSK Receiver
// Name:		RTLSDR.cpp
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
using namespace std;


//-------------------------------------CDemodulator----------------------------------------------------------------------------
class RTLSDR
{
private:
	rtlsdr_dev_t *dev;
public:
	RTLSDR(unsigned int samp_rate,unsigned int frequency,unsigned int gain);
};

#endif /* RTLSDR_H_ */
