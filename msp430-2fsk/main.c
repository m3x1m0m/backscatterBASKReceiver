/** 2FSK backscatter on MSP430 FR5969.
 *
 * Currently implements three different values of Delta f, which are listed below.
 *
 * DELTA_F_1238 corresponds to 1.238 MHz shift with a deviation of 190 kHz.
 * Set the carrier to 2480.9399M.
 *
 * DELTA_F_1506 corresponds to 1.506 MHz shift with a deviation of 188 kHz.
 * Set the carrier to 2480.6721M.
 *
 * DELTA_F_2091 corresponds to 2.091 MHz shift with a deviation of 182 kHz.
 * Set the carrier to 2480.0871M.
 */

#include <msp430.h>

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "lcg.h"

#define NODE_ID 1
#define DELTA_F DELTA_F_2091
#define BAUDRATE 1000

#include "main.h"

//static uint8_t packet_hdr[HEADER_LEN] = {0xaa, 0xaa, 0xaa, 0xaa, 0x01, 0x0b, 0x51, 0xde, 0x00};
static uint8_t packet_bits[MAX_PACKET_LEN*8];

static volatile struct {
	unsigned int bit_i;
	unsigned int nbits;
} txstate;


#pragma vector=TIMER0_A0_VECTOR
__interrupt void timer_a_bitbuf() {
	P1OUT ^= BIT2;
	if (txstate.bit_i < txstate.nbits) {
		P1OUT |= BIT0;

		if (packet_bits[txstate.bit_i]) {
			TBCCR0 = TIMERB_1_PERIOD-1;
			TBCCR1 = TIMERB_1_MID;
		} else {
		    TBCCR0 = 0;
		}
		txstate.bit_i++;
	} else {
		P1OUT &= ~BIT0;
		TBCCR0 = 0;
	}
}


/** Convert an array of bytes into an array of bits.
 *
 * Returns the number of bits in the destination array.
 */
int bytebuf_to_bitbuf(const uint8_t *bytebuf, unsigned int len, uint8_t *bitbuf) {
	unsigned int i, j, k;

	uint8_t message[512];

	k = 0;
	for (i=0;i<len;i++) {
        uint8_t byte = bytebuf[i];
        for (j=BIT7;j>=1;j>>=1) {

            if(byte & j){ //Manchester encoding 1 => 01
                message[k]=0;
                message[k+1]=1;
                bitbuf[k]=0;
                bitbuf[k+1]=1;
            }
            else{//Manchester encoding 0 => 10
                message[k]=1;
                message[k+1]=0;
                bitbuf[k]=1;
                bitbuf[k+1]=0;
            }
            k+=2;
        }
    }

	return k;
}


/** Prepare the packet bit buffer for the given payload.
 *
 * Returns number of bits to be transmitted.
 */
int prepare_packet(const uint8_t *payload, int payload_len) {
	int pkt_len = payload_len*2;//Time 2 because of manchester encoding
	int nbits = 0;

	if (pkt_len > MAX_PACKET_LEN) {
		// Packet too long.
		return 0;
	}

	//packet_hdr[HEADER_LEN-1] = payload_len;
	//nbits += bytebuf_to_bitbuf(packet_hdr, HEADER_LEN, packet_bits);
	//nbits += bytebuf_to_bitbuf(payload, payload_len, packet_bits + nbits);
	nbits += bytebuf_to_bitbuf(payload, payload_len, packet_bits);


	return nbits;
}


/** Fills a payload buffer with a sequence of bytes.
 */
int prn_payload(uint8_t *data, int len) {
	int i=0;
	//uint8_t string[13]="hello world!";
	//string[12]="\0";
	const char *string = "Hello World";

	//for (i=0;i<12;i++) {

	while (string[i]!= '\0'){//writing the data
	    data[i] = string[i];
	    i++;
	}

	//In this case the data does not take all the space allocated for it (64 characters)
	//The end of the data corresponds to data[i] = '\n';

	return i+1; // i + 1 for "\0"

}

/** Fills a payload buffer with ones.
 */
int prn_sendOnes(uint8_t *data, int len) {
    int i=0;


    while (i < len){
        // Just write 1s which will be encoded to 101010...
        data[i] = 0xFF;
        i++;
    }

    return i;

}

/** Set the CPU's clock frequency.
 *
 * The actual frequency is set at compile time using CPU_FREQ.
 */
void set_cpu_freq() {
#if CPU_FREQ == CPU_FREQ_8MHZ
    CSCTL0_H = 0xA5;
    CSCTL1 = DCOFSEL_6;
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;
    CSCTL0_H = 0;
#elif CPU_FREQ == CPU_FREQ_24MHZ
    FRCTL0 = 0xA500 | ((1) << 4);
    CSCTL0_H = 0xA5;
    CSCTL1 = DCOFSEL_6 | DCORSEL;
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;
    CSCTL0_H = 0;
#else
	#error "Invalid CPU frequency."
#endif
}


/** Initialize the MSP430's configurable pins.
 */
void init_pins() {
    /* SMCLK on P3.4, for validating CPU clock freq. */
    P3DIR |= BIT4;
    P3SEL1 |= BIT4;

    /* LED on P1.0 */
    P1SEL0 &= ~BIT0;
    P1SEL1 &= ~BIT0;
    P1DIR |= BIT0;

    /* GPIO on P1.2, used to show bit timing for debug purposes */
    P1SEL0 &= ~BIT2;
    P1SEL1 &= ~BIT2;
    P1DIR |= BIT2;

    /* TimerB1 on P1.4 (secondary function, output) */
    P1SEL0 |= BIT4;
    P1SEL1 &= ~BIT4;
    P1DIR |= BIT4;
}


/** Initialize timers to generate signals and control bit transmissions.
 */
void init_timers() {
    /* Setup TimerB, which will generate the signals. */
    TBCCR0 = 0;
    TBCCR1 = 0;
    TBCCTL1 = OUTMOD_7;
    TBCCTL0 = 0;
    TBCTL = TBSSEL_2 | TBCLR | MC_1;

    /* Setup TimerA, which will control bit transmission. */
    TA0CCR0 = CPU_FREQ/BAUDRATE - 1;
    TA0CCTL0 = CCIE;
    TA0CTL = TASSEL_2 | TACLR | MC_1;
}


void delay_ms(int ms) {

	while (ms) {
		__delay_cycles(CYCLES_PER_MS);
		ms--;
	}
}


int main(void) {

    WDTCTL = WDTPW | WDTHOLD;
    PM5CTL0 &= ~LOCKLPM5;

    set_cpu_freq();
    init_pins();
    init_timers();
    __enable_interrupt();


    uint8_t message[32];   //message to be send with  : 0xFF | SYNC | SIZE | DATA




    while (1) {

    /* First byte of message : 0xFF */
    //message[0]=0xFF;

    /* Second Byte of message SYNC (no idea what that is so I just put 0*/
    //message[1]= 0;

    /* third byte of message : SIZE (in bytes) of the message without manchester encoding) */
    //message[2]=prn_payload(message+3, sizeMessage-3);

    prn_sendOnes(message,32);


    /* Transmit packet. */
    	txstate.nbits = 0;
    	txstate.bit_i = 0;

    txstate.nbits = prepare_packet(message, 32);

    	/* Wait for transmission to complete. */
	    while (txstate.bit_i < txstate.nbits) {
	    		/* Busy wait. */
	    }

	    delay_ms(300);

	}

    return 0;
}
