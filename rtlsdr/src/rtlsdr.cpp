//============================================================================
// Name        : rtlsdr.cpp
// Author      : Elmar van Rijnswou
// Version     :
// Copyright   : Innoseis
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#ifndef _WIN32
#include <stdio.h>
#include <cstdio>
#include <stdlib.h>
#endif
#include "convenience/convenience.h"
#include "rtl-sdr.h"

#define DEFAULT_SAMPLE_RATE	2400000
#define DEFAULT_BUF_LENGTH		(16 * 16384)
#define MINIMAL_BUF_LENGTH		512
#define MAXIMAL_BUF_LENGTH	(256 * 16384)
static uint32_t bytes_to_read = 0;

using namespace std;

static rtlsdr_dev_t *dev = NULL;
int main() {
	int dev_index = verbose_device_search((char*) "0");
	int r = rtlsdr_open(&dev, (uint32_t) dev_index);
	if (r < 0) {
		fprintf(stderr, "Failed to open rtlsdr device #%d.\n", dev_index);
		exit(1);
	}

	uint32_t frequency = 626000000;
	uint32_t samp_rate = DEFAULT_SAMPLE_RATE;

	verbose_set_sample_rate(dev, samp_rate);

	/* Set the frequency */
	verbose_set_frequency(dev, frequency);

	verbose_gain_set(dev, 10);

	verbose_reset_buffer(dev);
	int n_read;
	uint32_t out_block_size = DEFAULT_BUF_LENGTH;
	uint8_t * buffer = new uint8_t[out_block_size];
	bool whileLoop = true;
	while (whileLoop) {
		r = rtlsdr_read_sync(dev, buffer, out_block_size, &n_read);
		if (r < 0) {
			fprintf(stderr, "WARNING: sync read failed.\n");
			break;
		}

		if ((bytes_to_read > 0) && (bytes_to_read < (uint32_t) n_read)) {
			n_read = bytes_to_read;
			whileLoop = false;
		}
		//FILE * file = stdout;

		if (fwrite(buffer, 1, n_read, stdout) != (size_t) n_read) {
			fprintf(stderr, "Short write, samples lost, exiting!\n");
			break;
		}

		if ((uint32_t) n_read < out_block_size) {
			fprintf(stderr, "Short read, samples lost, exiting!\n");
			break;
		}

		if (bytes_to_read > 0)
			bytes_to_read -= n_read;
	}
	delete buffer;
	return 0;
}
