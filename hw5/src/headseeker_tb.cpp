#include "headseeker.h"
#include <fstream>
#include <iostream>
#include <stdio.h>

int main(){
	hls::stream<ap_axiu<32,0,0,0> > msg_in("msg_in");
	hls::stream<ap_axiu<32,0,0,0> > msg_out("msg_out");
	char dataTemp;

	ap_axiu<32,0,0,0> in_axis;
	ap_axiu<32,0,0,0> out_axis;

	ap_uint<32> msg_send[N]={\
	0x33736F69, 0x45453231, 0x36323032, 0x30303432, 0x39485735, 0x73644545, 0x66736173, 0x64337765,\
	0x33736F69, 0x77654545, 0x32313632, 0x30323030, 0x34323948, 0x57357364, 0x45456673, 0x61736433,\
	0x45453231, 0x36323032, 0x30303432, 0x39485735, 0x73644545, 0x66736173, 0x64337765, 0x33736F69,\
	0x736F6977, 0x65454532, 0x31363230, 0x32303034, 0x32394857, 0x35736445, 0x45667361, 0x73643333};
	// 3soi, EE21, 6202, 0042, 9HW5, sdEE, fsas, d3we
	// 3soi, weEE, 2162, 0200, 429H, W5sd, EEfs, asd3
	// EE21, 6202, 0042, 9HW5, sdEE, fsas, d3we, 3soi
	// soiw, eEE2, 1620, 2004, 29HW, 5sdE, Efsa, sd33
	ap_uint<32> msg_recv[N+5];

	for(int i=0; i<7; i++)
	{
		in_axis.data = msg_send[i];
		in_axis.last = 0;
		msg_in.write(in_axis);
	}
	in_axis.data = msg_send[7];
	in_axis.last = 1;
	msg_in.write(in_axis);
	in_axis.data = 0x23232323;//the word after tlast=1 is #### and it can be neglected.
	in_axis.last = 0;
	msg_in.write(in_axis);

	for(int i=8; i<15; i++)
	{
		in_axis.data = msg_send[i];
		in_axis.last = 0;
		msg_in.write(in_axis);
	}
	in_axis.data = msg_send[15];
	in_axis.last = 1;
	msg_in.write(in_axis);
	in_axis.data = 0x23232323;
	in_axis.last = 0;
	msg_in.write(in_axis);

	for(int i=16; i<23; i++)
	{
		in_axis.data = msg_send[i];
		in_axis.last = 0;
		msg_in.write(in_axis);
	}
	in_axis.data = msg_send[23];
	in_axis.last = 1;
	msg_in.write(in_axis);
	in_axis.data = 0x23232323;
	in_axis.last = 0;
	msg_in.write(in_axis);

	for(int i=24; i<31; i++)
	{
		in_axis.data = msg_send[i];
		in_axis.last = 0;
		msg_in.write(in_axis);
	}
	in_axis.data = msg_send[31];
	in_axis.last = 1;
	msg_in.write(in_axis);
	in_axis.data = 0x23232323;
	in_axis.last = 0;
	msg_in.write(in_axis);

	for(int i=0; i<N+5; i++)
	{
		headseeker(msg_in, msg_out);
		out_axis = msg_out.read();
		msg_recv[i] = out_axis.data;
	}

	for(int i=0; i<N+5; i++)
	{
		printf("%c%c%c%c%c\n", \
				(char)msg_recv[i].range(31,24), (char)msg_recv[i].range(23,16), \
				(char)msg_recv[i].range(15,8), (char)msg_recv[i].range(7,0));
	}
	return 0;
}
