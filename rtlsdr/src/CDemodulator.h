//----------------------------------------------------------------------------------------------------------------------------
// Project:    	Backscatter BPSK Receiver
// Name:		CDemodulator.h
// Author:		Maximilian Stiefel
// Date:		07.03.2017
//
// Description:
//
//----------------------------------------------------------------------------------------------------------------------------

#ifndef CDemodulator_H
#define CDemodulator_H

//-------------------------------------Libraries-------------------------------------------------------------------------------

#include "rtl-sdr.h"

using namespace std;

//-------------------------------------Libraries-------------------------------------------------------------------------------

#define DEFAULT_BUF_LENGTH	(16 * 16384)
#define MINIMAL_BUF_LENGTH	512
#define MAXIMAL_BUF_LENGTH	(256 * 16384)

//-------------------------------------CDemodulator----------------------------------------------------------------------------
class CDemodulator
{
private:
	rtlsdr_dev_t *dev;		// Device handle
public:
	CDemodulator(unsigned int samp_rate,unsigned int frequency,unsigned int gain);
	void continuousReadout();
};

#endif
