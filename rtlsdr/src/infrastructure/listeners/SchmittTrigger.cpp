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
#include "../messages/ProcessedSampMess.h"
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
SchmittTrigger::SchmittTrigger(bool idebug, float ithreshold):Listener() {
	//Variables
	message::RawSampMess msg;

	//Action
	setSensitive(Sensitive( { message::MessageTypes::get().getType(&msg) }));

	initializeFIR(MY_COEFFICIENTS_FILE1, &coefficientsFilt1, &numTaps1);					// First filter before downsampling
	initializeFIR(MY_COEFFICIENTS_FILE2, &coefficientsFilt2,  &numTaps2);					// Second filter after downsampling

	std::cout << "First filter has " << numTaps1 << " taps." << std::endl;
	std::cout << "Second filter has " << numTaps2 << " taps." << std::endl;

	rawFile.open(MY_RAW_FILE, std::ios::out);								// Open files for logging
	filteredFile.open(MY_FILTERED_FILE, std::ios::out);
	binaryFile.open(MY_BINARY_FILE, std::ios::out);

	debug = idebug;
	threshold = ithreshold;
	std::cout << "Threshold: " << threshold << std::endl;
}

//-------------------------------------convertSample--------------------------------------------------------------------------
cmplsampfl_t SchmittTrigger::convertSample(uint8_t in_real, uint8_t in_imag){
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
	RawSampMess *msg_recv = NULL;
	ProcessedSampMess *msg_2send = NULL;
	float *fastBuffer = NULL;
	float *slowBuffer = NULL;
	unsigned int *intBuffer = NULL;
	unsigned int numSamps = 0;
	static unsigned int m = 0;

	//Action
	msg_recv = (RawSampMess*) message;
	numSamps = msg_recv->getSize()/2;

	if(!debug){
		fastBuffer = new float[numSamps];
		slowBuffer = new float[numSamps/MY_DECIMATION_FACTOR];
		intBuffer = new unsigned int[numSamps/MY_DECIMATION_FACTOR];
		while(i<msg_recv->getSize()){
			real=msg_recv->removeSample();
			imag=msg_recv->removeSample();
			fastBuffer[j] = rectify(convertSample(real,imag));								// Convert sample
			i+=2;
			j++;
		}
		filterFIR(fastBuffer, coefficientsFilt1, numSamps, numTaps1);						// First filter
		downSample(fastBuffer, slowBuffer, numSamps, MY_DECIMATION_FACTOR);					// Calm your live
		dumpFloats2File(rawFile,slowBuffer,numSamps/MY_DECIMATION_FACTOR);
		filterFIR(slowBuffer, coefficientsFilt2, numSamps/MY_DECIMATION_FACTOR, numTaps2);	// Second filter
		dumpFloats2File(filteredFile,slowBuffer,numSamps/MY_DECIMATION_FACTOR);
		msg_2send = new ProcessedSampMess(msg_recv->getSampleRate(), numSamps);
		for(unsigned int m=0;m<numSamps/MY_DECIMATION_FACTOR;m++){
			intBuffer[m] = trigger(&slowBuffer[m]);
			msg_2send->addSample(intBuffer[m]);
		}
		dumpInts2File(binaryFile,intBuffer,numSamps/MY_DECIMATION_FACTOR);
		std::cout << "Message nu.:" << ++m << std::endl;
	} else{																					// Check if FIR is implemented correctly
			std::cout << "Currently no implementation available :'(" << std::endl;
	}
	delete[] fastBuffer;
	delete[] slowBuffer;
}

//-------------------------------------dumpData2File---------------------------------------------------------------------------
void SchmittTrigger::dumpFloats2File(std::ofstream &myfile, float *floatBuffer, unsigned int length){
	// Variables
	unsigned int i = 0;

	//Action
	if(myfile.is_open()){
		i = 0;
		while(i < length){
			myfile << floatBuffer[i] << std::endl;
			i++;
		}
	} else
		std::cout << "File " << myfile << " is not opened." << std::endl;
}

//-------------------------------------dumpData2File---------------------------------------------------------------------------
void SchmittTrigger::dumpCmplx2File(std::ofstream &myfile, cmplsampfl_t *floatBuffer, unsigned int length){
	// Variables
	unsigned int i = 0;

	//Action
	if(myfile.is_open()){
		i = 0;
		while(i < length){
			myfile << floatBuffer[i].real << "," << floatBuffer[i].imag << std::endl;
			i++;
		}
	} else
		std::cout << "File " << myfile << " is not opened." << std::endl;
}

//-------------------------------------dumpData2File---------------------------------------------------------------------------
void SchmittTrigger::dumpInts2File(std::ofstream &myfile, unsigned int *intBuffer, unsigned int length){
	// Variables
	unsigned int i = 0;

	//Action
	if(myfile.is_open()){
		i = 0;
		while(i < length){
			myfile << intBuffer[i] << std::endl;
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
	delete [] coefficientsFilt1;
	delete [] coefficientsFilt2;
}
//-------------------------------------complexFIR-----------------------------------------------------------------------------
void SchmittTrigger::complexFIR(cmplsampfl_t *floatBuffer, float *filterCoefficients, unsigned int length, unsigned int num_taps){
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

//-------------------------------------filterFIR------------------------------------------------------------------------------
void SchmittTrigger::filterFIR(float *floatBuffer, float *filterCoefficients, unsigned int length, unsigned int num_taps){
	// Variables
	float y;
	float *reg;
	std::string line;

	// Action
	reg = new float[num_taps];												// Get some space for filter coefficients

	for(unsigned int j=0; j<num_taps; j++){
		reg[j] = 0.0;															// Initialize the delay registers.
	}
	for(unsigned int j=0; j<length; j++){
		for(unsigned int k=num_taps; k>1; k--)reg[k-1] = reg[k-2];			// Shift register values
		reg[0] = floatBuffer[j];
		y = 0.0;															// Produce new output
		for(unsigned int k=0; k<num_taps; k++){
			y += filterCoefficients[k] * reg[k];
		}
		floatBuffer[j] = y;													// Save new output
	}
	delete [] reg;															// Clean after yourself
}
//-------------------------------------initializeFIR--------------------------------------------------------------------------
void SchmittTrigger::initializeFIR(char* file, float **filterCoefficients, unsigned int *num_taps){
	//Variables
	std::ifstream myfile(file);
	std::string line;
	unsigned int start_pos = 0;
	unsigned int end_pos = 0;
	std::stringstream lineStream;
	std::string oneCoeff;
	std::vector<float> filterCoefficients_tmp;

	//Action
	if (myfile.is_open()){
			getline(myfile,line);											// Read one line
			start_pos = line.find('[') + 1;									// Remove shit around coefficients
			end_pos = line.find(']');
			line = line.substr(start_pos,end_pos-start_pos);
			lineStream.str(line);
			while(std::getline(lineStream,oneCoeff,',')){					// Read CSV values from filter design tool
				filterCoefficients_tmp.push_back(atof(oneCoeff.c_str()));
			}
			*num_taps = filterCoefficients_tmp.size();
			myfile.close();
			(*filterCoefficients) = new float[*num_taps];						// Get coefficients out of the vector
			for(unsigned int i=0; i<*num_taps;i++){
				(*filterCoefficients)[i]=filterCoefficients_tmp[i];
			}
	} else
		std::cout << "Unable to open coefficients file: " << file << std::endl;
}

//-------------------------------------trigger--------------------------------------------------------------------------------
unsigned int SchmittTrigger::trigger(float *floatBuffer){
	// Variables

	// Action
	if( threshold < *floatBuffer)
		return 1;
	else
		return 0;
}
//-------------------------------------rectify--------------------------------------------------------------------------------
float SchmittTrigger::rectify(cmplsampfl_t floatBuffer){
	// Variables

	// Actions
	return ( sqrt(floatBuffer.real*floatBuffer.real + floatBuffer.imag*floatBuffer.imag));
}
//-------------------------------------complexDownSample----------------------------------------------------------------------
void SchmittTrigger::complexDownSample(cmplsampfl_t *inStream, cmplsampfl_t *outStream, unsigned int length, unsigned int factor){
	// Variables
	unsigned int j = 0;

	// Action
	for(unsigned int i=0; i<length;i++){
		if(!(i%factor) && ( j < (length/factor)) ){
			outStream[j] = inStream[i];					// Only take every factorth sample
			j++;
		}
	}
}
//-------------------------------------downSample-----------------------------------------------------------------------------
void SchmittTrigger::downSample(float *inStream, float *outStream, unsigned int length, unsigned int factor){
	// Variables
	unsigned int j = 0;

	// Action
	for(unsigned int i=0; i<length;i++){
		if(!(i%factor) && ( j < (length/factor)) ){
			outStream[j] = inStream[i];					// Only take every factorth sample
			j++;
		}
	}
}

} /* namespace listener */
} /* namespace infrastructure */
} /* namespace backscatter */
