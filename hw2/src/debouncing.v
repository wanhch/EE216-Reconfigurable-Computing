module debouncing (
    input      CLK      , 
    input      ButtonIn ,
    output reg ButtonOut);

reg [2:0] counter;
localparam threshold = 3'd5;
reg [1:0] State;
localparam WAIT_START = 2'd0;
localparam COUNTING   = 2'd1;
localparam WAIT_END   = 2'd2;

always @(posedge CLK) begin
    case (State)
        WAIT_START: begin
            counter <= 3'd0;
            ButtonOut <= 1'b0;
            if (ButtonIn == 1'b1) begin
                State <= COUNTING;
                counter <= 3'd1;
            end
        end
        COUNTING: begin
            if (ButtonIn == 1'b1) begin
                counter <= counter + 1;
            end else begin
                State <= WAIT_START;
            end
            if (counter >= threshold-1) begin
                ButtonOut <= 1'b1;
                State <= WAIT_END;
            end
        end
        WAIT_END: begin
            ButtonOut <= 1'b0;
            if (ButtonIn == 1'b0) 
                State <= WAIT_START;
        end
        default: begin
            State <= WAIT_START;
            ButtonOut <= 1'b0;
        end
    endcase
end
endmodule