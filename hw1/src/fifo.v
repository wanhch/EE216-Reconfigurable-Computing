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

assign empty = ~(|fifo_cnt);
assign full = fifo_cnt[3];

always @(posedge clk) begin
    if (rst_n == 1'b0) begin
        fifo_cnt <= 4'd0;
        wp       <= 3'd0;
        rp       <= 3'd0;
        buf_out  <= 8'd0;
    end else begin
        if (wr_en == 1'b1) begin
            if (full == 1'b0) begin
                if (wp == 3'd7) begin
                    wp = 3'd0;
                end else begin 
                    wp = wp + 1;
                end
                mem[wp] <= buf_in;
                fifo_cnt = fifo_cnt + 1;
            end

        end

        if (rd_en == 1'b1) begin
            if (empty == 1'b0) begin
                if (rp == 3'd7) begin
                    rp = 3'd0;
                end else begin
                    rp = rp + 1;
                end
                buf_out <= mem[rp];
                fifo_cnt = fifo_cnt - 1;
            end
        end
    end
end
endmodule