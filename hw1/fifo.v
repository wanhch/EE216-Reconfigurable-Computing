module fifo (
    input clk, 
    input rst_n, 
    input [7:0] buf_in, 
    output reg [7:0] buf_out, 
    output empty, 
    output full, 
    output reg [3:0] fifo_cnt);