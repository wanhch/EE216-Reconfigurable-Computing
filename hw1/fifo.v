module fifo (
    input            clk    , 
    input            rst_n  , 
    input            wr_en  , 
    input            rd_en  ,
    input [7:0]      buf_in , 
    output reg [7:0] buf_out, 
    output           empty  , 
    output           full   , 
    output reg [3:0] fifo_cnt);

reg [7:0] mem [0:7];
reg [2:0] wp;
reg [2:0] rp;

always @(fifo_cnt) begin
    if (fifo_cnt == 4'd0) 
        empty <= 1'b1;
    else 
        empty <= 1'b0;
    if (fifo_cnt == 4'd8)
        full <= 1'b1;
    else 
        full <= 1'b0;
end

always @(posedge clk) begin
    if (rst_n == 1'b1) begin
        buf_out  <= 8'd0;
        fifo_cnt <= 4'd0;
        wp       <= 3'd0;
        rp       <= 3'd0;
    end else begin
        if (wr_en == 1'b1) begin
            if (full == 0'b0) begin
                if (wp == 3'd7) begin
                    wp <= 3'd0;
                    mem[0] <= buf_in;
                else begin 
                    wp <= wp + 1;
                    mem[wp + 1] <= buf_in;
                end
                fifo_cnt <= fifo_cnt + 1;
            end
        end

        if (rd_en == 1'b1) begin
            if (empty == 0'b0) begin
                if (rp == 3'd7) begin
                    rp <= 3'd0;
                    buf_out <= mem[0];
                end else begin
                    rp <= rp + 1;
                    buf_out <= mem[rp + 1];
                end
                fifo_cnt <= fifo_cnt - 1;
            else begin
                buf_out <= 8'd0;
            end
        end
    end
end