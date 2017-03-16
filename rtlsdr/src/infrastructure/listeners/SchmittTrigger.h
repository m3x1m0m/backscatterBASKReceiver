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

//-------------------------------------Namespace------------------------------------------------------------------------------
namespace backscatter {
namespace infrastructure {
namespace listener{

//-------------------------------------Defines--------------------------------------------------------------------------------
#define MY_RAW_FILE (char*)"raw.csv"
#define MY_FILTERED_FILE (char*)"filtered.csv"
#define MY_COEFFICIENTS_FILE (char*)"coefficients.csv"

//-------------------------------------Typedefs-------------------------------------------------------------------------------

typedef struct cmplsampfl_t{					// Complex sample as float
		float real;
		float imag;
}cmplsampfl_t;

//-------------------------------------SchmittTrigger-------------------------------------------------------------------------
class SchmittTrigger: public Listener {
public:
	SchmittTrigger();
	virtual void receiveMessage(message::Message * message) override;
	virtual ~SchmittTrigger();
private:
	cmplsampfl_t convertSample(uint8_t in_real, uint8_t in_imag, bool debug);
	std::vector<float> filterCoefficients;
	void showADCData(uint8_t in_real, uint8_t in_imag);
	void dumpFloats2File(char *filename, cmplsampfl_t *floatBuffer, unsigned int length);
	void filterFIR(cmplsampfl_t *floatBuffer, unsigned int length);

};

}
} /* namespace infrastructure */
} /* namespace backscatter */

#endif /* INFRASTRUCTURE_LISTENERS_SCHMITTT_H_ */
