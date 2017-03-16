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

int main() {
	using namespace backscatter::infrastructure;
	using namespace backscatter::infrastructure::listener;
	MessageBus *bus = new MessageBus();
	RTLSDR myRTLSDR(1.2e6, 632.2e6, 400, bus);
	SchmittTrigger *mySchmittTrigger = new SchmittTrigger(false,19.0);
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
