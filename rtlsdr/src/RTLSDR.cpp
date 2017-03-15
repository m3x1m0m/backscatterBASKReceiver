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

	cout << "Initialization done." << endl;
}
//-------------------------------------Destructor-----------------------------------------------------------------------------
RTLSDR::~RTLSDR() {
	rtlsdr_cancel_async(dev);
}

//-------------------------------------Callback-------------------------------------------------------------------------------
void RTLSDR::processCallback(unsigned char *buf, uint32_t len, void *ctx) {
	//Variables
	uint32_t i = 0;
	RawSampMess *msg;
	msg = new RawSampMess(samp_rate, MY_BUFFER_LENGTH);

	//Action
	while (i < len) {
		msg->addSample(buf[i]);
		i++;
	}
	msgBus->pushMessage(msg);
	cout << "Pushed samples to bus." << endl;
}

//-------------------------------------Workaround4Callback--------------------------------------------------------------------
void RTLSDR::rtlsdrCallback(unsigned char * buf, uint32_t len, void * ctx) {
	RTLSDR * rtlsdr = (RTLSDR*) ctx;
	rtlsdr->processCallback(buf, len, ctx);
}

//-------------------------------------continuousReadout----------------------------------------------------------------------
void RTLSDR::continuousReadout() {
	// Variables
	int r = 0;
	uint32_t buffer_length = MY_BUFFER_LENGTH;

	// Action
	verbose_reset_buffer(dev);								// Empty HW buffer

	cout << "Buffer size: " << buffer_length << " byte" << endl;
	cout << "Starting to read." << endl;
	// Start async readout
	r = rtlsdr_read_async(dev, rtlsdrCallback, this, 0, buffer_length);
	if (r < 0) {
		fprintf(stderr, "WARNING: async read failed.\n");
	}
}
} /* namespace backscatter */
