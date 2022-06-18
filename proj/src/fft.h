#ifndef FFT_H_
#define FFT_H_

#include <time.h>
#include <sys/time.h>

typedef float DTYPE;
typedef int INTTYPE;

#define M 10 			/*Number of Stages = Log2N */
#define SIZE 1024		/* SIZE OF FFT */
#define SIZE2 SIZE>>1
#define N 16

//#pragma SDS data zero_copy(X_R[0:SIZE], X_I[0:SIZE], OUT_R[0:SIZE], OUT_I[0:SIZE])
void fft_hw(DTYPE X_R[SIZE], DTYPE X_I[SIZE], DTYPE OUT_R[SIZE], DTYPE OUT_I[SIZE]);

#endif
