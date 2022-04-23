#include "hls_stream.h"
#include "ap_int.h"
#include "hls_stream.h"
#include "ap_axi_sdata.h"
#include <ap_shift_reg.h>

#define N 32
#define EE216 0x4545323136

void headseeker(hls::stream<ap_axiu<32,0,0,0> >& msg_in, hls::stream<ap_axiu<32,0,0,0> >& msg_out);
