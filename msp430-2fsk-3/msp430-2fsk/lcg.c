#include <stdint.h>

#define A 1664525ul
#define C 1013904223ul
#define RAND_MAX ((1ul << 31) - 1)

uint32_t rseed = 0;
 
void lcg_seed(uint32_t x) {
	rseed = x;
}
 
 
uint32_t lcg_rand() {
	rseed = (rseed * A + C) & RAND_MAX;
	return rseed;
}


#if LCG_BUILD_MAIN
#include <stdio.h>


int main(int argc, char **argv) {
    int i;
    uint32_t s;

    for (s=0;s<256;s++) {
        lcg_seed(s);
        printf("%02x ", s);
        for (i=0;i<128;i++) {
            printf("%02x ", (uint8_t) lcg_rand());
        }
        printf("\n");
    }
    return 0;
}
#endif
