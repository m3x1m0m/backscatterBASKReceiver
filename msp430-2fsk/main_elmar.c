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
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "lcg.h"
#include "driverlib.h"

uint8_t one = 0;

#define NODE_ID 1
#define DELTA_F DELTA_F_2091
#define BAUDRATE 2400

#include "main.h"

static uint8_t packet_hdr[HEADER_LEN] = { 0xaa, 0xaa, 0xaa, 0xaa, 0x93, 0x0b,
                                          0x51, 0xde, 0x00 };
static uint8_t packet_bits[MAX_PACKET_LEN * 8] = {0};

static union crc_16
{
    uint16_t crc;
    uint8_t array[2];
};

static union payLd
{
    unsigned long long cnt;
    uint8_t array[8];
};

#define CRC16 0x8005

uint16_t crc16(char *data_p, uint16_t length)
{
    char data[] = "123456789";
    uint8_t i;

    //Set the CRC seed
    CRC_setSeed(CRC_BASE, CRC16);
    printf("Adding: ");
    for (i = 0; i < sizeof(data); i++)
    {
        printf("%c", data[i]);
        //Add all of the values into the CRC signature
        CRC_set8BitData(CRC_BASE, data[i]);
    }

    printf("\n result: %04X reversed: %04X data: %d\n", CRC_getResult(CRC_BASE),
           CRC_getResultBitsReversed(CRC_BASE), CRC_getData(CRC_BASE));

    return CRC_getResult(CRC_BASE);
}

static volatile struct
{
    unsigned int bit_i;
    unsigned int nbits;
} txstate;

#pragma vector=TIMER0_A0_VECTOR
__interrupt void timer_a_bitbuf()
{
    P1OUT ^= BIT2;
    if (txstate.bit_i < txstate.nbits)
    {
        P1OUT |= BIT0;

        //if (packet_bits[txstate.bit_i])
        if(one)
        {
            TBCCR0 = TIMERB_1_PERIOD - 1;
            TBCCR1 = TIMERB_1_MID;
        }
        txstate.bit_i++;
    }
    else
    {
        P1OUT &= ~BIT0;
        TBCCR0 = 0;
        //WDTCTL = 0;
    }
}

/** Convert an array of bytes into an array of bits.
 *
 * Returns the number of bits in the destination array.
 */
int bytebuf_to_bitbuf(const uint8_t *bytebuf, unsigned int len, uint8_t *bitbuf)
{
    unsigned int i, j, k;

    k = 0;
    for (i = 0; i < len; i++)
    {
        uint8_t byte = bytebuf[i];
        for (j = BIT7; j >= 1; j >>= 1)
        {
            bitbuf[k] = byte & j;
            k++;
        }
    }

    return k;
}

uint16_t bitCounter(uint8_t * data, uint8_t len)
{
    uint8_t i = 0, b;
    uint8_t ones = 0, zeros = 0;
    for (; i < len; i++)
    {
        uint8_t temp = data[i];
        for (b = 0; b < 8; b++)
        {
            if (temp & (1 << b))
                ones++;
            else
                zeros++;
        }
    }
    return (uint16_t) ones << 8 | zeros;
}

/** Prepare the packet bit buffer for the given payload.
 *
 * Returns number of bits to be transmitted.
 */
int prepare_packet(const uint8_t *payload, int payload_len)
{
    union crc_16 crc;
    int pkt_len = HEADER_LEN + payload_len + 2;
    int nbits = 0;

    if (pkt_len > MAX_PACKET_LEN)
    {
        // Packet too long.
        return 0;
    }

    packet_hdr[HEADER_LEN - 1] = payload_len + 2;
    nbits += bytebuf_to_bitbuf(packet_hdr, HEADER_LEN, packet_bits);
    nbits += bytebuf_to_bitbuf(payload, payload_len, packet_bits + nbits);
    return nbits;
}

/** Fills a payload buffer with a pseudo-random sequence of bytes.
 */
void prn_payload(uint8_t *payload, int len)
{
    static uint8_t payLd = 0;
    int i;
    for (i = 0; i < len; i++)
    {
        payload[i] = payLd + i; //(uint8_t) lcg_rand();
    }
    payLd++;
}

/** Set the CPU's clock frequency.
 *
 * The actual frequency is set at compile time using CPU_FREQ.
 */
void set_cpu_freq()
{
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
void init_pins()
{
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
void init_timers()
{
    /* Setup TimerB, which will generate the signals. */
    TBCCR0 = 0;
    TBCCR1 = 0;
    TBCCTL1 = OUTMOD_7;
    TBCCTL0 = 0;
    TBCTL = TBSSEL_2 | TBCLR | MC_1;

    /* Setup TimerA, which will control bit transmission. */
    TA0CCR0 = CPU_FREQ / BAUDRATE - 1;
    TA0CCTL0 = CCIE;
    TA0CTL = TASSEL_2 | TACLR | MC_1;
}

void delay_ms(int ms)
{

    while (ms)
    {
        __delay_cycles(CYCLES_PER_MS);
        ms--;
    }
}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    PM5CTL0 &= ~LOCKLPM5;

    set_cpu_freq();
    init_pins();
    init_timers();
    __enable_interrupt();

    uint8_t payload[32] = {0};
    uint8_t payloadLen = 2 + sizeof(long long);
    const uint16_t crcSeed = 0x8005;
    payload[1] = 0;         // counter
    uint8_t i = 0;
    union crc_16 crc;
    payload[0] = payloadLen;   // node id
    while (1)
    {
        /* Create random payload. */
        //lcg_seed(payload[1] | (NODE_ID << 4));
        CRC_setSeed(CRC_BASE, crcSeed);
        union payLd * payld = (union payLd *)&payload[2];
        payld->cnt++;
        //prn_payload(payload + 2, payloadLen - 2);

        /* Transmit packet. */
        //txstate.nbits = 0;
        txstate.bit_i = 0;
        txstate.nbits = prepare_packet(&payload[0], payloadLen);
        crc.crc = bitCounter(payld->array, sizeof(long long));
        txstate.nbits += bytebuf_to_bitbuf(crc.array, 2, packet_bits + txstate.nbits);
        txstate.nbits = 2400;

        /* Wait for transmission to complete. */
        //while(txstate.bit_i<txstate.nbits);

        delay_ms(1000);
        one = !one;
        //payload[1]++;
    }

    return 0;
}
