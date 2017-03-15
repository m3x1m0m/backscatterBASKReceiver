//----------------------------------------------------------------------------------------------------------------------------
// Project:    	Backscatter BPSK Receiver
// Name:		RTLSDR.cpp
// Author:		Maximilian Stiefel
// Date:		15.03.2017
//
// Description:
//
//----------------------------------------------------------------------------------------------------------------------------

//-------------------------------------Libraries------------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include "convenience/convenience.h"
#include "rtl-sdr.h"
#ifndef _WIN32
	#include <stdio.h>
	#include <stdlib.h>
#endif
#include "RTLSDR.h"												// Own class definitions

using namespace std;

//-------------------------------------CDemodulator---------------------------------------------------------------------------
RTLSDR::RTLSDR(unsigned int samp_rate,unsigned int frequency,unsigned int gain)
{
	// Variables
	int dev_index = 0;
	int r = 0;

	// Action
	dev = NULL;
	dev_index = verbose_device_search((char*) "0");						// Find device
	rtlsdr_open(&dev, (uint32_t) dev_index);							// Open device
	if (r < 0) {
		fprintf(stderr, "Failed to open rtlsdr device #%d.\n", dev_index);
		exit(1);
	}
	verbose_set_sample_rate(dev, samp_rate);							// Set sampling rate
	verbose_set_frequency(dev, frequency);								// Set frequency
	if (gain == 0) {													// Set gain
		verbose_auto_gain(dev);
	} else {
		gain = nearest_gain(dev, gain);
		verbose_gain_set(dev, gain);
	}
	cout << "Initialization done." << endl;
}
