/*
This is traditional 2-radix DIT FFT algorithm implementation.
INPUT:
	In_R, In_I[]: Real and Imag parts of Complex signal

OUTPUT:
	Out_R, Out_I[]: Real and Imag parts of Complex signal
*/

#include "fft.h"
#include "math.h"

void bit_reverse(DTYPE X_R[SIZE], DTYPE X_I[SIZE], DTYPE OUT_R[SIZE], DTYPE OUT_I[SIZE]);

unsigned int reverse_bits(unsigned int input) {
	int i, rev = 0;
	for (i = 0; i < M; i++) {
#pragma HLS PIPELINE II=1
		rev = (rev << 1) | (input & 1);
		input = input >> 1;
	}
	return rev;
}

void bit_reverse(DTYPE X_R[SIZE], DTYPE X_I[SIZE],
		 DTYPE OUT_R[SIZE], DTYPE OUT_I[SIZE]) {
	unsigned int reversed;
	DTYPE temp;
	for (int i = 0; i < SIZE; i++) {
		reversed = reverse_bits(i); // Find the bit reversed index
		if (i <= reversed) {
#pragma HLS PIPELINE II=2
			// Swap the real values
			temp = X_R[i];
			OUT_R[i] = X_R[reversed];
			OUT_R[reversed] = temp;
			// Swap the imaginary values
			temp = X_I[i];
			OUT_I[i] = X_I[reversed];
			OUT_I[reversed] = temp;
		}
	}
}

void fft_stage(int stage, DTYPE X_R[SIZE], DTYPE X_I[SIZE], DTYPE Out_R[SIZE], DTYPE Out_I[SIZE]) {
  int DFTpts = 1 << stage;    // DFT = 2^stage = points in sub DFT
  int numBF = DFTpts / 2;     // Butterfly WIDTHS in sub-DFT
  DTYPE e = -6.283185307178 / DFTpts;
  DTYPE a = 0.0;
  // Perform butterflies for j-th stage
 butterfly_loop:
  for (int j = 0; j < numBF; j++) {
    DTYPE c = cos(a);
    DTYPE s = sin(a);
    a = a + e;
    // Compute butterflies that use same W**k
  dft_loop:
    for (int i = j; i < SIZE; i += DFTpts) {
#pragma HLS PIPELINE II=2
#pragma HLS LOOP_TRIPCOUNT max=2
      int i_lower = i + numBF; // index of lower point in butterfly
      DTYPE temp_R = X_R[i_lower] * c - X_I[i_lower] * s;
      DTYPE temp_I = X_I[i_lower] * c + X_R[i_lower] * s;
      Out_R[i_lower] = X_R[i] - temp_R;
      Out_I[i_lower] = X_I[i] - temp_I;
      Out_R[i] = X_R[i] + temp_R;
      Out_I[i] = X_I[i] + temp_I;
    }
  }
}

void fft_stage1(int stage, DTYPE X_R[SIZE], DTYPE X_I[SIZE], DTYPE Out_R[SIZE], DTYPE Out_I[SIZE]) {
  int DFTpts = 1 << stage;    // DFT = 2^stage = points in sub DFT
  int numBF = DFTpts / 2;     // Butterfly WIDTHS in sub-DFT
  DTYPE e = -6.283185307178 / DFTpts;
  DTYPE a = 0.0;
  // Perform butterflies for j-th stage
 butterfly_loop:
  for (int j = 0; j < 512; j++) {
#pragma HLS PIPELINE II=4
    DTYPE c = cos(a);
    DTYPE s = sin(a);
    if ((j+1) % numBF == 0) {
    	a = 0;
    } else {
    	a = a + e;
    }
    // Compute butterflies that use same W**k
    int	i = j + numBF * (j / numBF);
      int i_lower = i + numBF; // index of lower point in butterfly
      DTYPE temp_R = X_R[i_lower] * c - X_I[i_lower] * s;
      DTYPE temp_I = X_I[i_lower] * c + X_R[i_lower] * s;
      Out_R[i_lower] = X_R[i] - temp_R;
      Out_I[i_lower] = X_I[i] - temp_I;
      Out_R[i] = X_R[i] + temp_R;
      Out_I[i] = X_I[i] + temp_I;
  }
}

void fft_hw(DTYPE X_R[SIZE], DTYPE X_I[SIZE], DTYPE OUT_R[SIZE], DTYPE OUT_I[SIZE])
{
#pragma HLS ARRAY_PARTITION variable=X_I complete dim=1
#pragma HLS ARRAY_PARTITION variable=OUT_I complete dim=1
#pragma HLS ARRAY_PARTITION variable=X_R complete dim=1
#pragma HLS ARRAY_PARTITION variable=OUT_R complete dim=1
#pragma HLS DATAFLOW
    DTYPE Stage_R[M][SIZE], Stage_I[M][SIZE];
#pragma HLS ARRAY_PARTITION variable=Stage_R complete dim=1
#pragma HLS ARRAY_PARTITION variable=Stage_I complete dim=1
    bit_reverse(X_R, X_I, Stage_R[0], Stage_I[0]);
    int midpoint = M - 8;
stage_loop:
    for (int stage = 1; stage < midpoint; stage++){
#pragma HLS UNROLL
        fft_stage(stage, Stage_R[stage-1], Stage_I[stage-1], Stage_R[stage], Stage_I[stage]);
    }
    for (int stage = midpoint; stage < M; stage++) {
#pragma HLS UNROLL
    	fft_stage1(stage, Stage_R[stage-1], Stage_I[stage-1], Stage_R[stage], Stage_I[stage]);
    }
    fft_stage1(M, Stage_R[M-1], Stage_I[M-1], OUT_R, OUT_I);
}


/*=======================END: FFT=========================*/
