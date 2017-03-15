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
#include "RTLSDR.h"

using namespace std;

int main() {
	RTLSDR MyRTLSDR(1.8e6,105.616e6,400);
	return 0;
}
