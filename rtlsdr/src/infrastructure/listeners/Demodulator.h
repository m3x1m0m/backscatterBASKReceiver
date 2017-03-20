//----------------------------------------------------------------------------------------------------------------------------
// Project:    	Backscatter BASK Demodulator
// Name:		Demodulator.h
// Author:		Maximilian Stiefel
// Date:		15.03.2017
//
// Description:
//
//----------------------------------------------------------------------------------------------------------------------------

#ifndef INFRASTRUCTURE_LISTENERS_SCHMITTT_H_
#define INFRASTRUCTURE_LISTENERS_SCHMITTT_H_

//-------------------------------------Libraries------------------------------------------------------------------------------
#include "Listener.h"
#include "../MessageBus.h"
#include <fstream>

//-------------------------------------Namespace------------------------------------------------------------------------------
namespace backscatter {
namespace infrastructure {
namespace listener{

//-------------------------------------Defines--------------------------------------------------------------------------------
#define MY_RAW_FILE (char*)"raw.csv"
#define MY_FILTERED_FILE (char*)"filtered.csv"
#define MY_BINARY_FILE (char*)"binary.csv"
#define MY_COEFFICIENTS_FILE1 (char*)"../filters/low_filter_250e3_10e3.csv"
#define MY_COEFFICIENTS_FILE2 (char*)"../filters/low_filter_25e3_0.5e3.csv"
#define MY_DECIMATION_FACTOR 10						// Factor for downsampling

//-------------------------------------Typedefs-------------------------------------------------------------------------------

typedef struct cmplsampfl_t{						// Complex sample as float
		float real;
		float imag;
}cmplsampfl_t;

//-------------------------------------Demodulator-------------------------------------------------------------------------
class Demodulator: public Listener {
public:
	Demodulator(bool idebug, float ithreshold,  MessageBus* ibus);
	virtual void receiveMessage(message::Message * message) override;
	virtual ~Demodulator();
private:
	float *coefficientsFilt1;						// Two different filters have to be used
	float *coefficientsFilt2;
	unsigned int numTaps1;
	unsigned int numTaps2;

	std::ofstream rawFile;							// Different file handels to save data to look at it
	std::ofstream filteredFile;
	std::ofstream binaryFile;
	bool debug;
	float threshold;
	MessageBus* msgBus;

	void showADCData(uint8_t in_real, uint8_t in_imag);
	void dumpFloats2File(std::ofstream &myfile, float *floatBuffer, unsigned int length);
	void dumpCmplx2File(std::ofstream &myfile, cmplsampfl_t *floatBuffer, unsigned int length);
	void dumpInts2File(std::ofstream &myfile, unsigned int *intBuffer, unsigned int length);
	void complexFIR(cmplsampfl_t *floatBuffer, float *filterCoefficients, unsigned int length,unsigned int num_taps);
	void filterFIR(float *floatBuffer, float *filterCoefficients, unsigned int length, unsigned int num_taps);
	void initializeFIR(char* file, float **filterCoefficients, unsigned int *num_taps);
	void complexDownSample(cmplsampfl_t *inStream, cmplsampfl_t *outStream, unsigned int length, unsigned int factor);
	void downSample(float *inStream, float *outStream, unsigned int length, unsigned int factor);
	void rectify(cmplsampfl_t *inStream, float *outStream, unsigned int length);
	unsigned int trigger(float *floatBuffer);
	cmplsampfl_t convertSample(uint8_t *in_real, uint8_t *in_imag);
};

}
} /* namespace infrastructure */
} /* namespace backscatter */

#endif /* INFRASTRUCTURE_LISTENERS_SCHMITTT_H_ */
