#ifndef MAIN_H_
#define MAIN_H_

#define DELTA_F_1238	1238
#define DELTA_F_1506	1506
#define DELTA_F_2091	2091
#define DELTA_F_0219	219
#define DELTA_F_0490	490

#define CPU_FREQ_24MHZ	24000000
#define CPU_FREQ_8MHZ	 8000000

#if DELTA_F == DELTA_F_1238
	#define CPU_FREQ CPU_FREQ_8MHZ
	#define TIMERB_1_PERIOD	6				// 1.333 MHz
	#define TIMERB_0_PERIOD	7				// 1.143 MHz
#elif DELTA_F == DELTA_F_1506
	#define CPU_FREQ	CPU_FREQ_24MHZ
	#define TIMERB_1_PERIOD 15				// 1.6 MHz
	#define TIMERB_0_PERIOD 17				// 1.412 MHz
#elif DELTA_F == DELTA_F_2091
	#define CPU_FREQ	CPU_FREQ_24MHZ
	#define TIMERB_1_PERIOD 11				// 2.182 MHz
	#define TIMERB_0_PERIOD 12				// 2 MHz
#elif DELTA_F == DELTA_F_0219
	#define CPU_FREQ	CPU_FREQ_8MHZ
	#define TIMERB_1_PERIOD 36				//  222.222 kHz
	#define TIMERB_0_PERIOD 37				//  216.216 kHz
#elif DELTA_F == DELTA_F_0490
	#define CPU_FREQ	CPU_FREQ_24MHZ
	#define TIMERB_1_PERIOD 48				//  500.0 kHz
	#define TIMERB_0_PERIOD 50				//  480.0 kHz
#else
	#error "Invalid value for Delta f"
#endif

#define TIMERB_1_MID (TIMERB_1_PERIOD/2)
#define TIMERB_0_MID (TIMERB_0_PERIOD/2)
#define CYCLES_PER_MS (CPU_FREQ/1000)

#define MAX_PACKET_LEN 128
#define HEADER_LEN 9							// 4 preamble, 4 sync, 1 payload len

#endif /* MAIN_H_ */
