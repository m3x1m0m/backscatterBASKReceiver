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
#include "RTLSDR.h"														// Own class definitions
#include "./infrastructure/messages/RawSampMess.h"

//-------------------------------------Namespaces-----------------------------------------------------------------------------
namespace backscatter {
using namespace std;
using namespace backscatter::infrastructure;
using namespace backscatter::infrastructure::message;

//-------------------------------------Constructor----------------------------------------------------------------------------
RTLSDR::RTLSDR(unsigned int isamp_rate, unsigned int ifrequency,
		unsigned int igain, MessageBus* bus) {
	// Variables
	int dev_index = 0;
	int r = 0;

	// Action
	samp_rate = isamp_rate;
	frequency = ifrequency;
	gain = igain;
	msgBus = bus;
	dev = NULL;
	dev_index = verbose_device_search((char*) "0");				// Find device
	rtlsdr_open(&dev, (uint32_t) dev_index);					// Open device
	if (r < 0) {
		fprintf(stderr, "Failed to open rtlsdr device #%d.\n", dev_index);
		exit(1);
	}
	verbose_set_sample_rate(dev, samp_rate);				// Set sampling rate
	verbose_set_frequency(dev, frequency);						// Set frequency
	if (gain == 0) {												// Set gain
		verbose_auto_gain(dev);
	} else {
		gain = nearest_gain(dev, gain);
		verbose_gain_set(dev, gain);
	}

	verbose_reset_buffer(dev);								// Empty HW buffer

	cout << "Initialization done." << endl;
}
//-------------------------------------Destructor-----------------------------------------------------------------------------
RTLSDR::~RTLSDR() {

}

//-------------------------------------continuousReadout----------------------------------------------------------------------
void RTLSDR::continuousReadout(){
	// Variables
	int r = 0;
	int n_read = 0;
	unsigned int len = MY_BUFFER_LENGTH;
	uint8_t *buf = new uint8_t[MY_BUFFER_LENGTH];
	unsigned int i = 0;
	RawSampMess *msg;

	// Action
	cout << "Buffer size: " << MY_BUFFER_LENGTH << " byte" << endl;
	cout << "Starting to read." << endl;
	while(1){
		msg = new RawSampMess(samp_rate, MY_BUFFER_LENGTH);
		i = 0;
		r = rtlsdr_read_sync(dev, buf, len, &n_read);
		if (r < 0) {
			fprintf(stderr, "WARNING: sync read failed.\n");
		}
		while (i < len){
			msg->addSample(buf[i]);
			i++;
		}
		msgBus->pushMessage(msg);
		cout << "RTLSDR: Pushed samples to bus." << endl;
	}
}
} /* namespace backscatter */
