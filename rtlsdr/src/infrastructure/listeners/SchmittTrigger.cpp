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
#include <cmath>

namespace backscatter {
namespace infrastructure {
namespace listener {

//-------------------------------------Constructor----------------------------------------------------------------------------
SchmittTrigger::SchmittTrigger(bool idebug):Listener() {
	//Variables
	message::RawSampMess msg;
	std::ifstream myfile (MY_COEFFICIENTS_FILE);
	std::string line;
	unsigned int start_pos = 0;
	unsigned int end_pos = 0;
	std::stringstream lineStream;
	std::string oneCoeff;
	std::vector<float> filterCoefficients_tmp;

	//Action
	setSensitive(Sensitive( { message::MessageTypes::get().getType(&msg) }));

	if (myfile.is_open()){
			getline(myfile,line);											// Read one line
			start_pos = line.find('[') + 1;									// Remove shit around coefficients
			end_pos = line.find(']');
			line = line.substr(start_pos,end_pos-start_pos);
			lineStream.str(line);
			while(std::getline(lineStream,oneCoeff,',')){					// Read CSV values from filter design tool
				filterCoefficients_tmp.push_back(atof(oneCoeff.c_str()));
			}
			num_taps = filterCoefficients_tmp.size();
			std::cout << "Number of taps: " << filterCoefficients_tmp.size() << std::endl;
			myfile.close();
			filterCoefficients = new float[num_taps];						// Get coefficients out of the vector
			for(unsigned int i=0; i<filterCoefficients_tmp.size();i++){
				filterCoefficients[i]=filterCoefficients_tmp[i];
			}
	} else
		std::cout << "Unable to open file.";

	rawFile.open(MY_RAW_FILE, std::ios::out);								// Open files for logging
	filteredFile.open(MY_FILTERED_FILE, std::ios::out);
	binaryFile.open(MY_BINARY_FILE, std::ios::out);

	debug = idebug;
}

//-------------------------------------convertSample--------------------------------------------------------------------------
cmplsampfl_t SchmittTrigger::convertSample(uint8_t in_real, uint8_t in_imag, bool debug){
	cmplsampfl_t oneSample;
	oneSample.real = ( (float) in_real)-127.5;								// First byte is from the in phase ADC
	oneSample.imag = ( (float) in_imag)-127.5;								// Second byte is from the quadrature ADC acc. to the internet
	if(debug){
		std::cout << "Real (Float): " << oneSample.real << "\n";
		std::cout << "Imaginary (Float): " << oneSample.imag << "\n";
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
	unsigned int mess_size = 0;
	static unsigned int m = 0;

	//Action
	mess = (RawSampMess*) message;
	mess_size= mess->getSize();

	if(!debug){
		floatBuffer = new cmplsampfl_t[mess_size];
		while(i<mess_size){
			real=mess->removeSample();
			imag=mess->removeSample();
			floatBuffer[j] = convertSample(real,imag,false);						// Convert sample
			i+=2;
			j++;
		}
		std::cout << "Message Nu.:" << ++m << std::endl;
		dumpFloats2File(rawFile,floatBuffer,mess_size/2);							// Save raw data
		filterFIR(floatBuffer,mess_size/2);											// Filter
		dumpFloats2File(filteredFile,floatBuffer,mess_size/2);						// Save filtered data
		for(unsigned int m=0;m<mess_size/2;m++){
			floatBuffer[m] = trigger(&floatBuffer[m]);
		}
	} else{
		floatBuffer = new cmplsampfl_t[num_taps];
		while(i<num_taps){
					floatBuffer[i].real=0;
					floatBuffer[i].imag=0;
					if(i > 10) {
						floatBuffer[i].real=10;
						floatBuffer[i].imag=10;
					}
					i++;
				}
		std::cout << "Debug Mode! Message Nu.:" << ++m << std::endl << num_taps << std::endl;
		if(m<=1){
			dumpFloats2File(rawFile,floatBuffer,num_taps);								// Save raw data
			filterFIR(floatBuffer,num_taps);											// Filter
			dumpFloats2File(filteredFile,floatBuffer,num_taps);							// Save filtered data
		}
	}
	delete[] floatBuffer;
}

//-------------------------------------dumpData2File---------------------------------------------------------------------------
void SchmittTrigger::dumpFloats2File(std::ofstream &myfile, cmplsampfl_t *floatBuffer, unsigned int length){
	// Variables
	unsigned int i = 0;

	//Action
	if(myfile.is_open()){
		i = 0;
		while(i < length){
			myfile << floatBuffer[i].real << "," << floatBuffer[i].imag << std::endl;
			if(!debug)
				i+=MY_DECIMATION_FACTOR;									// Downsampling here necessary
			else
				i++;
		}
	} else
		std::cout << "File " << myfile << " is not opened." << std::endl;
}

//-------------------------------------showADCData----------------------------------------------------------------------------
void SchmittTrigger::showADCData(uint8_t in_real, uint8_t in_imag){
	std::cout << "Real (ADC): " << ((int)in_real) << "\n";					// First byte is from the inphase ADC
	std::cout << "Imaginary (ADC): " << ((int)in_imag) << "\n";				// Second byte is from the quadrature ADC acc. to the internet
}

//-------------------------------------Destructor-----------------------------------------------------------------------------
SchmittTrigger::~SchmittTrigger() {
	rawFile.close();
	filteredFile.close();
	binaryFile.close();
	delete [] filterCoefficients;
}
//-------------------------------------filterFIR------------------------------------------------------------------------------
void SchmittTrigger::filterFIR(cmplsampfl_t *floatBuffer, unsigned int length){
	// Variables
	cmplsampfl_t y;
	cmplsampfl_t *reg;
	std::string line;

	// Action
	reg = new cmplsampfl_t[num_taps];										// Get some space for filter coefficients

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
	delete [] reg;															// Clean after yourself
}

//-------------------------------------trigger--------------------------------------------------------------------------------
unsigned int SchmittTrigger::trigger(cmplsampfl_t *floatBuffer){
	// Variables

	// Action
	if(threshold>sqrt(floatBuffer->real^2))
		return 1;
	else
		return 0;
}
} /* namespace infrastructure */
} /* namespace backscatter */
