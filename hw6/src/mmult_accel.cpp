/**********
Copyright (c) 2018, Xilinx, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors
may be used to endorse or promote products derived from this software
without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**********/

#include "mmult_accel.h"

void mmult_hw(int in1[DATA_SIZE * DATA_SIZE], int in2[DATA_SIZE * DATA_SIZE], int out[DATA_SIZE * DATA_SIZE], int dim)
{
#pragma HLS DATAFLOW
    const int c_size = DATA_SIZE;
    int in1buf[DATA_SIZE], in2buf[DATA_SIZE], temp[DATA_SIZE];
#pragma HLS ARRAY_PARTITION variable=temp cyclic factor=256 dim=1
#pragma HLS ARRAY_PARTITION variable=in2buf block factor=256 dim=1
#pragma HLS ARRAY_PARTITION variable=in1buf cyclic factor=256 dim=1

    for (int i = 0; i < DATA_SIZE; ++i) {
        for (int j = 0; j < DATA_SIZE; ++j) {
#pragma HLS UNROLL factor=256
            in1buf[j] = in1[i * dim + j];
        }
        for (int j = 0; j < DATA_SIZE; ++j) {
            for (int k = 0; k < DATA_SIZE; ++k) {
#pragma HLS UNROLL factor=256
                in2buf[k] = in2[k * dim + j];
            }
            int result = 0;
            for (int k = 0; k < DATA_SIZE; ++k) {
#pragma HLS UNROLL factor=256
                temp[k] = in1buf[k] * in2buf[k];
            }
            for (int k = 0; k < DATA_SIZE; ++k) {
#pragma HLS UNROLL factor=256
                result += temp[k];
            }
            out[i * dim + j] = result;
        }
    }
}
