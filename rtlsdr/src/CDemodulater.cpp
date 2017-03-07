//----------------------------------------------------------------------------------------------------------------------------
// Project:    	Backscatter BPSK Receiver
// Name:		CDemodulator.h
// Author:		Maximilian Stiefel
// Date:		07.03.2017
//
// Description:
//
//----------------------------------------------------------------------------------------------------------------------------

//-------------------------------------Libraries------------------------------------------------------------------------------
#include <iostream>
#include <unistd.h>
#include "convenience/convenience.h"
#include "rtl-sdr.h"
#ifndef _WIN32
	#include <stdio.h>
	#include <stdlib.h>
#endif
#include "CDemodulator.h"										//own class definitions

using namespace std;

//-------------------------------------CDemodulator---------------------------------------------------------------------------
CDemodulator::CDemodulator(unsigned int samp_rate,unsigned int frequency,unsigned int gain)
{
	// Variables
	int dev_index = 0;
	int r = 0;

	// Action
	dev = NULL;
	dev_index = verbose_device_search((char*) "0");				// Find device
	rtlsdr_open(&dev, (uint32_t) dev_index);					// Open device
	if (r < 0) {
		fprintf(stderr, "Failed to open rtlsdr device #%d.\n", dev_index);
		exit(1);
	}
	verbose_set_sample_rate(dev, samp_rate);					// Set sampling rate
	verbose_set_frequency(dev, frequency);						// Set frequency
	verbose_gain_set(dev, gain);								// Set gain
}

//-------------------------------------continuousReadout-----------------------------------------------------------------------
void CDemodulator::continuousReadout(){
	// Variables
	bool whileLoop = true;
	int n_read = 0;
	int r = 0;
	uint32_t buffer_length = DEFAULT_BUF_LENGTH*4;
	uint8_t * buffer = new uint8_t[buffer_length];

	// Action
	verbose_reset_buffer(dev);										// Empty HW buffer

	while (whileLoop) {
		r = rtlsdr_read_sync(dev, buffer, buffer_length, &n_read);	// Perform readout
		if (r < 0) {
			fprintf(stderr, "WARNING: sync read failed.\n");
			break;
		}

		cout << "Length of read data: " << n_read << "Byte" << "\n";
		usleep(200*1);
	}
	delete buffer;
}

