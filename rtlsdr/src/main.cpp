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
#include "infrastructure/listeners/Decoder.h"
#include "RTLSDR.h"

using namespace std;
using namespace backscatter;
using namespace backscatter::infrastructure;
	using namespace backscatter::infrastructure::listener;

int main(int argc, char* argv[]) {
	if(argc < 5){
		cout << "Wrong usage! Right usage: " << endl;
		cout << "./rtlsdr fsamp ftuned gain threshold" << endl;
		return 1;
	}

	MessageBus *bus = new MessageBus();
	RTLSDR myRTLSDR((unsigned int)atof(argv[1]), (unsigned int)atof(argv[2]), 10*(unsigned int)atof(argv[3]), bus);
	Demodulator *myDemodulator = new Demodulator(false,(unsigned int)atof(argv[4]), bus);
	Decoder *myDecoder = new Decoder();
	bus->addListener(myDemodulator);
	bus->addListener(myDecoder);
	thread t1(&MessageBus::runLoop, bus);
	thread t2(&RTLSDR::continuousReadout, &myRTLSDR);
	char c;
	cin >> c;
	while (bus->isRunning());
	t1.join();
	t2.join();
	delete bus;
	return 0;
}
