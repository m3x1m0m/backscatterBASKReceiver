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

//-------------------------------------Typedefs---------------------------------------------------------------------------------

typedef struct cmplsampfl_t{					// Complex sample as float
		float real;
		float imag;
}cmplsampfl_t;

//-------------------------------------Defines---------------------------------------------------------------------------------

#define MY_BUFFER_LENGTH 100*(2*1024)			// 2 Bytes per sample (I,Q)

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
	void resetFile(char *filename);
	void filterFIR(cmplsampfl_t *floatBuffer, uint32_t buffer_length,char *filename);
	void schmittTrig(cmplsampfl_t *floatBuffer, uint8_t *resultBuffer);
	static void rtlsdrCallback(unsigned char *buf, uint32_t len, void *ctx);
};

#endif
