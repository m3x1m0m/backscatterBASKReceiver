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
	if (gain == 0) {													// Set gain
		verbose_auto_gain(dev);
	} else {
		gain = nearest_gain(dev, gain);
		verbose_gain_set(dev, gain);
	}
}
//-------------------------------------CDemodulator---------------------------------------------------------------------------
void CDemodulator::rtlsdrCallback(unsigned char *buf, uint32_t len, void *ctx)
{
	if (ctx) {
		cout << "Read data asynchronously." << endl;
		cout << "Buffer Length:" << len << endl;

	}
}

//-------------------------------------continuousReadout-----------------------------------------------------------------------
void CDemodulator::continuousReadout(){
	// Variables
	bool whileLoop = true;
	int r = 0;
	uint32_t buffer_length = MY_BUFFER_LENGTH;
	uint8_t *intBuffer = new uint8_t [buffer_length];
	cmplsampfl_t *floatBuffer = new cmplsampfl_t [buffer_length];

	// Action
	verbose_reset_buffer(dev);											// Empty HW buffer

	cout << "Buffer size: " << buffer_length << " byte" << endl;
	cout << "Starting to read." << endl;

	resetFile((char*)"raw.csv");
	resetFile((char*)"filtered.csv");

	r = rtlsdr_read_async(dev, rtlsdrCallback, (void *)1, 0,buffer_length);
	if (r < 0) {
				fprintf(stderr, "WARNING: async read failed.\n");
	}

	while (whileLoop){
	}
	delete intBuffer;
	delete floatBuffer;
}

//-------------------------------------convertSample---------------------------------------------------------------------------
cmplsampfl_t CDemodulator::convertSample(uint8_t in_real, uint8_t in_imag, bool debug){
	cmplsampfl_t oneSample;
	oneSample.real = ( (float) in_real)-127.5;						// First byte is from the in phase ADC
	oneSample.imag = ( (float) in_imag)-127.5;						// Second byte is from the quadrature ADC acc. to the internet
	if(debug){
		cout << "Real (Float): " << oneSample.real << "\n";
		cout << "Imaginary (Float): " << oneSample.real << "\n";
	}
	return oneSample;
}

//-------------------------------------showADCData-----------------------------------------------------------------------------
void CDemodulator::showADCData(uint8_t in_real, uint8_t in_imag){
	cout << "Real (ADC): " << ((int)in_real) << "\n";					// First byte is from the inphase ADC
	cout << "Imaginary (ADC): " << ((int)in_imag) << "\n";				// Second byte is from the quadrature ADC acc. to the internet
}
//-------------------------------------dumpData2File---------------------------------------------------------------------------
void CDemodulator::dumpFloats2File(char *filename, cmplsampfl_t *floatBuffer, uint32_t buffer_length){
	// Variables
	ofstream myfile;
	uint32_t i = 0;

	//Action
	myfile.open (filename, ios::app);
	i = 0;
	while(i < buffer_length){
		myfile << floatBuffer[i].real << "," << floatBuffer[i].imag << endl;
		i++;
	}
	myfile.close();
}
//-------------------------------------resetFile------------------------------------------------------------------------------
void CDemodulator::resetFile(char *filename){
	// Variables
	ofstream myfile;

	//Action
	 myfile.open (filename, ios::out);
	 myfile << "";
	 myfile.close();
}
//-------------------------------------filterFIR------------------------------------------------------------------------------
void CDemodulator::filterFIR(cmplsampfl_t *floatBuffer, uint32_t buffer_length,char *filename){
	// Variables
	int i = 0;
	cmplsampfl_t y;
	cmplsampfl_t *reg;
	string line;
	float *filterCoeff;
	uint32_t num_taps = 0;

	// Action
	ifstream myfile (filename);
	if (myfile.is_open())
	{
		while ( getline (myfile,line) )	num_taps++;
		myfile.clear();
		myfile.seekg(0, ios::beg);											// Put the file pointer back to the beginning
		filterCoeff = new float[num_taps];
		i = 0;
		while ( getline (myfile,line) ){
			filterCoeff[i] = atof(line.c_str());							// Convert string to float
			i++;
		}
		myfile.close();

		reg = new cmplsampfl_t[num_taps];									// Get some space for filter coefficients

		for(uint32_t j=0; j<num_taps; j++){
			reg[j].imag = 0.0; 												// Initialize the delay registers.
			reg[j].real = 0.0;
		}

		for(uint32_t j=0; j<num_taps; j++)
		{
			for(uint32_t k=num_taps; k>1; k--)reg[k-1] = reg[k-2];			// Shift register values
			reg[0] = floatBuffer[j];

			y.real = 0.0;													// Produce new output
			y.imag = 0.0;
			for(uint32_t k=0; k<num_taps; k++){
				y.real += filterCoeff[k] * reg[k].real;
				y.imag += filterCoeff[k] * reg[k].imag;
			}

			floatBuffer[j].real = y.real;									// Save new output
			floatBuffer[j].imag = y.imag;
		}
		delete reg;															// Clean after yourself
	}
	else cout << "Unable to open file.";
}

void CDemodulator::schmittTrig(cmplsampfl_t *floatBuffer, uint8_t *resultBuffer){

}


