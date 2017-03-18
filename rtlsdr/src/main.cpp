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
#include "RTLSDR.h"
#include "./infrastructure/listeners/SchmittTrigger.h"

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
	SchmittTrigger *mySchmittTrigger = new SchmittTrigger(false,(unsigned int)atof(argv[4]));
	bus->addListener(mySchmittTrigger);
	thread t1(&MessageBus::runLoop, bus);
	thread t2(&RTLSDR::continuousReadout, &myRTLSDR);
	char c;
	std::cin >> c;
	bus->stop();
	while (bus->isRunning())
		;
	t1.join();
	t2.join();
	delete bus;
	return 0;
}
