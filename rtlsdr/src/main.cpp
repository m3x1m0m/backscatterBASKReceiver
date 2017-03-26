//----------------------------------------------------------------------------------------------------------------------------
// Project:    	Backscatter BPSK Receiver
// Name:		main.cpp
// Author:		Maximilian Stiefel
// Date:		15.03.2017
//
// Description:
//
//----------------------------------------------------------------------------------------------------------------------------

#include <iostream>
#include <thread>

#include "infrastructure/listeners/Demodulator.h"
#include "infrastructure/listeners/StupidDecoder.h"
#include "RTLSDRSim.h"

using namespace std;
using namespace backscatter;
using namespace backscatter::infrastructure;
using namespace backscatter::infrastructure::listener;

int main(int argc, char* argv[]) {
	//Variables
	unsigned int fsamp, ftuned, gain, threshold;

	// Action
	if(argc < 5){
		cout << "Wrong usage! Right usage: " << endl;
		cout << "./rtlsdr fsamp ftuned gain threshold" << endl;
		return 1;
	}

	fsamp = (unsigned int)atof(argv[1]);
	ftuned = (unsigned int)atof(argv[2]);
	gain = 10*(unsigned int)atof(argv[3]);
	threshold = (unsigned int)atof(argv[4]);

	MessageBus *bus = new MessageBus();
	//RTLSDR myRTLSDR(fsamp, ftuned, gain, bus);
	RTLSDRSim myRTLSDR(bus,fsamp);
	Demodulator *myDemodulator = new Demodulator(false,threshold, bus);
	//Decoder *myDecoder = new Decoder();
	bus->addListener(myDemodulator);
	//bus->addListener(new StupidDecoder((unsigned int)atof(argv[1])));
	thread t1(&MessageBus::runLoop, bus);
	thread t2(&RTLSDRSim::continuousReadout, &myRTLSDR,true,10);
	char c;
	cin >> c;
	while (bus->isRunning());
	t1.join();
	t2.join();
	delete bus;
	return 0;
}
