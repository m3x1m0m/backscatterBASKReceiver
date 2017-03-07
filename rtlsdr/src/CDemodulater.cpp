//----------------------------------------------------------------------------------------------------------------------------
// Project:    	Backscatter BPSK Receiver
// Name:		CDemodulator.h
// Author:		Maximilian Stiefel
// Date:		07.03.2017
//
// Description:
//
//----------------------------------------------------------------------------------------------------------------------------

//-------------------------------------Libraries------------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include "convenience/convenience.h"
#include "rtl-sdr.h"
#ifndef _WIN32
	#include <stdio.h>
	#include <stdlib.h>
#endif
#include "CDemodulator.h"												// Own class definitions

using namespace std;

//-------------------------------------CDemodulator---------------------------------------------------------------------------
CDemodulator::CDemodulator(unsigned int samp_rate,unsigned int frequency,unsigned int gain)
{
	// Variables
	int dev_index = 0;
	int r = 0;

	// Action
	dev = NULL;
	dev_index = verbose_device_search((char*) "0");						// Find device
	rtlsdr_open(&dev, (uint32_t) dev_index);							// Open device
	if (r < 0) {
		fprintf(stderr, "Failed to open rtlsdr device #%d.\n", dev_index);
		exit(1);
	}
	verbose_set_sample_rate(dev, samp_rate);							// Set sampling rate
	verbose_set_frequency(dev, frequency);								// Set frequency
	verbose_gain_set(dev, gain);										// Set gain
}

//-------------------------------------continuousReadout-----------------------------------------------------------------------
void CDemodulator::continuousReadout(){
	// Variables
	bool whileLoop = true;
	int n_read = 0;
	int r = 0;
	uint32_t buffer_length = MY_BUFFER_LENGTH;
	uint8_t *intBuffer = new uint8_t [buffer_length];
	cmplsampfl_t *floatBuffer = new cmplsampfl_t [buffer_length];
	uint32_t i,j = 0;

	// Action
	verbose_reset_buffer(dev);											// Empty HW buffer

	while (whileLoop) {
		r = rtlsdr_read_sync(dev, intBuffer, buffer_length, &n_read);	// Perform readout
		if (r < 0) {
			fprintf(stderr, "WARNING: sync read failed.\n");
			break;
		}
		i = 0;
		j = 0;
		while(i < buffer_length){
			floatBuffer[j] = convertSample(intBuffer[i],intBuffer[i+1],false);
			i+=2;
			j++;
		}
		whileLoop=false;
	}
	dumpFloats2File((char*)"test.csv",floatBuffer,buffer_length/2);
	delete intBuffer;
	delete floatBuffer;
}

//-------------------------------------convertSample---------------------------------------------------------------------------
cmplsampfl_t CDemodulator::convertSample(uint8_t in_real, uint8_t in_imag, bool debug){
	cmplsampfl_t oneSample;
	oneSample.out_real = ( (float) in_real)-127.5;						// First byte is from the in phase ADC
	oneSample.out_imag = ( (float) in_imag)-127.5;						// Second byte is from the quadrature ADC acc. to the internet
	if(debug){
		cout << "Real (Float): " << oneSample.out_real << "\n";
		cout << "Imaginary (Float): " << oneSample.out_real << "\n";
	}
	return oneSample;
}

//-------------------------------------showADCData-----------------------------------------------------------------------------
void CDemodulator::showADCData(uint8_t in_real, uint8_t in_imag){
	cout << "Real (ADC): " << ((int)in_real) << "\n";					// First byte is from the inphase ADC
	cout << "Imaginary (ADC): " << ((int)in_imag) << "\n";				// Second byte is from the quadrature ADC acc. to the internet
}
//-------------------------------------dumpData2File---------------------------------------------------------------------------
void CDemodulator::dumpFloats2File(char *filename, cmplsampfl_t *floatBuffer,uint32_t buffer_length){
	// Variables
	ofstream myfile;
	uint32_t i = 0;

	//Action
	 myfile.open (filename);
	 i = 0;
	 while(i < buffer_length){
		 myfile << floatBuffer[i].out_real << "," << floatBuffer[i].out_imag << endl;
		 i++;
	 }
	 myfile.close();
}


