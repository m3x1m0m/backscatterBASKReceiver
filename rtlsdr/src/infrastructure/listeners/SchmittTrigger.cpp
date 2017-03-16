//----------------------------------------------------------------------------------------------------------------------------
// Project:    	Backscatter BPSK Receiver
// Name:		SchmittTrigger.h
// Author:		Maximilian Stiefel
// Date:		15.03.2017
//
// Description:
//
//----------------------------------------------------------------------------------------------------------------------------

//-------------------------------------Libraries------------------------------------------------------------------------------
#include "SchmittTrigger.h"
#include "../messages/RawSampMess.h"
#include "../messages/MessageTypes.h"
#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include <sstream>
#include <stdlib.h>

namespace backscatter {
namespace infrastructure {
namespace listener {

//-------------------------------------Constructor----------------------------------------------------------------------------
SchmittTrigger::SchmittTrigger():Listener() {
	//Variables
	message::RawSampMess msg;
	std::ifstream myfile (MY_COEFFICIENTS_FILE);
	std::string line;
	unsigned int start_pos = 0;
	unsigned int end_pos = 0;
	std::stringstream lineStream;
	std::string oneCoeff;

	//Action
	setSensitive(Sensitive( { message::MessageTypes::get().getType(&msg) }));

	if (myfile.is_open()){
			getline(myfile,line);											// Read one line
			start_pos = line.find('[') + 1;									// Remove shit around coefficients
			end_pos = line.find(']');
			line = line.substr(start_pos,end_pos-start_pos);
			lineStream.str(line);
			while(std::getline(lineStream,oneCoeff,',')){					// Read CSV values from filter design tool
			        filterCoefficients.push_back(atof(oneCoeff.c_str()));
			}
			std::cout << "Number of taps: " << filterCoefficients.size() << std::endl;
			myfile.close();
	} else std::cout << "Unable to open file.";
}

//-------------------------------------convertSample--------------------------------------------------------------------------
cmplsampfl_t SchmittTrigger::convertSample(uint8_t in_real, uint8_t in_imag, bool debug){
	cmplsampfl_t oneSample;
	oneSample.real = ( (float) in_real)-127.5;								// First byte is from the in phase ADC
	oneSample.imag = ( (float) in_imag)-127.5;								// Second byte is from the quadrature ADC acc. to the internet
	if(debug){
		std::cout << "Real (Float): " << oneSample.real << "\n";
		std::cout << "Imaginary (Float): " << oneSample.real << "\n";
	}
	return oneSample;
}

//-------------------------------------receiveMessage-------------------------------------------------------------------------
void SchmittTrigger::receiveMessage(message::Message* message) {
	//Variables
	uint8_t real = 0;
	uint8_t imag = 0;
	unsigned int i = 0,j = 0;
	RawSampMess *mess = NULL;
	cmplsampfl_t *floatBuffer;

	//Action
	mess = (RawSampMess*) message;
	floatBuffer = new cmplsampfl_t[mess->getSize()];
	while(i<mess->getSize()){
		real=mess->removeSample();
		imag=mess->removeSample();
		floatBuffer[j] = convertSample(real,imag,false);						// Convert sample
		i+=2;
		j++;
	}
	dumpFloats2File(MY_RAW_FILE,floatBuffer,mess->getSize()/2);					// Save raw data
	filterFIR(floatBuffer,mess->getSize());										// Filter
	dumpFloats2File(MY_FILTERED_FILE,floatBuffer,mess->getSize()/2);			// Save filtered data
	delete[] floatBuffer;
}

//-------------------------------------dumpData2File---------------------------------------------------------------------------
void SchmittTrigger::dumpFloats2File(char *filename, cmplsampfl_t *floatBuffer, unsigned int length){
	// Variables
	std::ofstream myfile;
	unsigned int i = 0;

	//Action
	myfile.open(filename, std::ios::app);
	i = 0;
	while(i < length){
		myfile << floatBuffer[i].real << "," << floatBuffer[i].imag << std::endl;
		i++;
	}
	myfile.close();
	//std::cout << "Samples dumped into " << filename << "." << std::endl;
}

//-------------------------------------showADCData----------------------------------------------------------------------------
void SchmittTrigger::showADCData(uint8_t in_real, uint8_t in_imag){
	std::cout << "Real (ADC): " << ((int)in_real) << "\n";					// First byte is from the inphase ADC
	std::cout << "Imaginary (ADC): " << ((int)in_imag) << "\n";				// Second byte is from the quadrature ADC acc. to the internet
}

//-------------------------------------Destructor-----------------------------------------------------------------------------
SchmittTrigger::~SchmittTrigger() {
	// TODO Auto-generated destructor stub
}
//-------------------------------------filterFIR------------------------------------------------------------------------------
void SchmittTrigger::filterFIR(cmplsampfl_t *floatBuffer, unsigned int length){
	// Variables
	cmplsampfl_t y;
	cmplsampfl_t *reg;
	std::string line;
	unsigned int num_taps = filterCoefficients.size();

	// Action
	reg = new cmplsampfl_t[num_taps];						// Get some space for filter coefficients

	for(unsigned int j=0; j<num_taps; j++){
			reg[j].imag = 0.0; 												// Initialize the delay registers.
			reg[j].real = 0.0;
	}
	for(unsigned int j=0; j<length; j++){
		for(unsigned int k=num_taps; k>1; k--)reg[k-1] = reg[k-2];			// Shift register values
		reg[0] = floatBuffer[j];
		y.real = 0.0;														// Produce new output
		y.imag = 0.0;
		for(unsigned int k=0; k<num_taps; k++){
			y.real += filterCoefficients[k] * reg[k].real;
			y.imag += filterCoefficients[k] * reg[k].imag;
		}
		floatBuffer[j].real = y.real;										// Save new output
		floatBuffer[j].imag = y.imag;
	}
	delete [] reg;														// Clean after yourself
}

}
} /* namespace infrastructure */
} /* namespace backscatter */
