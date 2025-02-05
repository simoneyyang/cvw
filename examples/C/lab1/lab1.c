#include <stdio.h>  // supports printf
#include "util.h"   // supports verify
//#include <stdint.h> // for longer ints

// Add two Q1.31 fixed point numbers
int add_q31(int a, int b) {
    int result = a + b;
    return result;
}

// Multiply two Q1.31 fixed point numbers
int mul_q31(int a, int b) {
    long value = (long)a*(long)b;
    int shift = (value << 1) >> 32;
    return shift;
    printf("mul_q31: a = %x, b = %x, res = %lx, result = %x\n", a, b, value, shift);

}

// low pass filter x with coefficients c, result in y
// n is the length of x, m is the length of c
// y[i] = c[0]*x[i] + c[1]*x[i+1] + ... + c[m-1]*x[i+m-1]
// inputs in Q1.31 format
void fir(int x[], int c[], int y[], int n, int m) {
	// TODO: your code here, use add_q31 and mul_q31
    for (int j = 0; j < n; ++j) {
        int sum = 0;
        for (int i = 0; i < m; ++i) {
            int part1 = c[i];
            // int negi = ~i + 1; // negative i
            // int ads = add_q31(j, negi);
            // int mred = add_q31(m, 0x11111111); // m - 1
            // int part2in = add_q31(ads, mred);
            // int part2out = x[part2in];
            int part2in = (j - i + (m - 1));
            int part2out = x[part2in];

            int res = mul_q31(part1, part2out);
            sum = add_q31(sum, res);
        }
        y[j] = sum;
    }
}

int main(void) {
    int32_t sin_table[20] = { // in Q1.31 format
        0x00000000, // sin(0*2pi/10)
        0x4B3C8C12, // sin(1*2pi/10)
        0x79BC384D, // sin(2*2pi/10)
        0x79BC384D, // sin(3*2pi/10)
        0x4B3C8C12, // sin(4*2pi/10)
        0x00000000, // sin(5*2pi/10)
        0xB4C373EE, // sin(6*2pi/10)
        0x8643C7B3, // sin(7*2pi/10)
        0x8643C7B3, // sin(8*2pi/10)
        0xB4C373EE, // sin(9*2pi/10)
        0x00000000, // sin(10*2pi/10)
        0x4B3C8C12, // sin(11*2pi/10)
        0x79BC384D, // sin(12*2pi/10)
        0x79BC384D, // sin(13*2pi/10)
        0x4B3C8C12, // sin(14*2pi/10)
        0x00000000, // sin(15*2pi/10)
        0xB4C373EE, // sin(16*2pi/10)
        0x8643C7B3, // sin(17*2pi/10)
        0x8643C7B3, // sin(18*2pi/10)
        0xB4C373EE  // sin(19*2pi/10)
    };  
    int lowpass[4] = {0x20000001, 0x20000002, 0x20000003, 0x20000004}; // 1/4 in Q1.31 format
    int y[17];
    int expected[17] = { // in Q1.31 format
        0x4fad3f2f,
        0x627c6236,
        0x4fad3f32,
        0x1e6f0e17,
        0xe190f1eb,
        0xb052c0ce,
        0x9d839dc6,
        0xb052c0cb,
        0xe190f1e6,
        0x1e6f0e12,
        0x4fad3f2f,
        0x627c6236,
        0x4fad3f32,
        0x1e6f0e17,
        0xe190f1eb,
        0xb052c0ce,
        0x9d839dc6
    };
    setStats(1);        // record initial mcycle and minstret
    fir(sin_table, lowpass, y, 20, 4);
    setStats(0);        // record elapsed mcycle and minstret
    for (int i=0; i<17; i++) {
        printf("y[%d] = %x\n", i, y[i]);
    }
    return verify(16, y, expected); 
// check the 1 element of s matches expected. 0 means success
}
