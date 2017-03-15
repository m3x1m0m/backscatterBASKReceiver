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
	RTLSDR myRTLSDR(1.8e6,105.616e6,400, bus);
	SchmittTrigger *mySchmittTrigger = new SchmittTrigger();
	bus->addListener(mySchmittTrigger);
	thread t1(&MessageBus::runLoop,bus);
	myRTLSDR.continuousReadout();
	while(bus->isRunning());
	t1.join();
	return 0;
}
