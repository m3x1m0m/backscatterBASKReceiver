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

//-------------------------------------Defines---------------------------------------------------------------------------------

#define MY_BUFFER_LENGTH (2*1024)					// 2 Bytes per sample (I,Q)

//-------------------------------------Typedefs---------------------------------------------------------------------------------

typedef uint64_t myfixedpt64_t;						// Fixed point number 64 bit
typedef struct cmplsampfl_t{						// Complex sample as float
		float out_real;
		float out_imag;
}cmplsampfl_t;

//-------------------------------------CDemodulator----------------------------------------------------------------------------
class CDemodulator
{
private:
	rtlsdr_dev_t *dev;							// Device handle
public:
	CDemodulator(unsigned int samp_rate,unsigned int frequency,unsigned int gain);
	void continuousReadout();
	cmplsampfl_t convertSample(uint8_t in_real, uint8_t in_imag, bool debug);
	void showADCData(uint8_t in_real, uint8_t in_imag);
	void dumpFloats2File(char *filename, cmplsampfl_t *floatBuffer,uint32_t buffer_length);
};

#endif
