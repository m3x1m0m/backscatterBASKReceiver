//----------------------------------------------------------------------------------------------------------------------------
// Project:    	Backscatter BPSK Receiver
// Name:		SchmittTrigger.h
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
#include <fstream>

//-------------------------------------Namespace------------------------------------------------------------------------------
namespace backscatter {
namespace infrastructure {
namespace listener{

//-------------------------------------Defines--------------------------------------------------------------------------------
#define MY_RAW_FILE (char*)"raw.csv"
#define MY_FILTERED_FILE (char*)"filtered.csv"
#define MY_BINARY_FILE (char*)"filtered.csv"
#define MY_COEFFICIENTS_FILE (char*)"coefficients.csv"
#define MY_DECIMATION_FACTOR 100					// Factor for downsampling of the stream which ends up in a file

//-------------------------------------Typedefs-------------------------------------------------------------------------------

typedef struct cmplsampfl_t{					// Complex sample as float
		float real;
		float imag;
}cmplsampfl_t;

//-------------------------------------SchmittTrigger-------------------------------------------------------------------------
class SchmittTrigger: public Listener {
public:
	SchmittTrigger(bool idebug);
	virtual void receiveMessage(message::Message * message) override;
	virtual ~SchmittTrigger();
private:
	cmplsampfl_t convertSample(uint8_t in_real, uint8_t in_imag, bool debug);
	float *filterCoefficients;
	unsigned int num_taps;
	std::ofstream rawFile;
	std::ofstream filteredFile;
	std::ofstream binaryFile;
	bool debug;
	float threshold;

	void showADCData(uint8_t in_real, uint8_t in_imag);
	void dumpFloats2File(std::ofstream &myfile, cmplsampfl_t *floatBuffer, unsigned int length);
	void filterFIR(cmplsampfl_t *floatBuffer, unsigned int length);
	unsigned int trigger(cmplsampfl_t *floatBuffer);
};

}
} /* namespace infrastructure */
} /* namespace backscatter */

#endif /* INFRASTRUCTURE_LISTENERS_SCHMITTT_H_ */
