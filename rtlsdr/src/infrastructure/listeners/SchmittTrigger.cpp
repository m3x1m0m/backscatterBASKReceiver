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

namespace backscatter {
namespace infrastructure {
namespace listener {

//-------------------------------------Constructor----------------------------------------------------------------------------
SchmittTrigger::SchmittTrigger():Listener() {
	message::RawSampMess msg;
	setSensitive(Sensitive( { message::MessageTypes::get().getType(&msg) }));
	floatBuffer = NULL;
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

	//Action
	mess = (RawSampMess*) message;
	floatBuffer = new cmplsampfl_t[mess->getSize()];
	while(i<mess->getSize()){
		real=mess->removeSample();
		imag=mess->removeSample();
		floatBuffer[j] = convertSample(real,imag,true);						// Convert sample
		i+=2;
		j++;
	}
	dumpFloats2File(MY_RAW_FILE,floatBuffer,mess->getSize()/2);
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
	std::cout << "Samples dumped into " << filename << "." << std::endl;
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

}
} /* namespace infrastructure */
} /* namespace backscatter */
