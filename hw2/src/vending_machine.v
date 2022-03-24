module vending_machine (
    input  Enable,
    input  RST,
    input  CLK,
    input  OneDollar,
    input  FiftyCents,
    input  TenCents,
    input  FiveCents,
    output Deliver,
    output [7:0] Money);

parameter Price = 125;

reg deliver_reg;
reg [7:0] money_reg;
reg State;
localparam READY = 1'b0;
localparam SALE  = 1'b1;

assign Deliver = deliver_reg;
assign Money = money_reg;

always @(posedge CLK, posedge RST) begin
    if (RST == 1'b0) begin
        money_reg <= 8'd0;
        deliver_reg <= 1'b0;
        State <= READY;
    end else begin
        case(State)
            READY: begin
                money_reg <= 8'd0;
                deliver_reg <= 1'b0;
                if (Enable == 1'b1)
                    State <= SALE;
            end
            SALE: begin
                if (OneDollar == 1'b1)
                    money_reg <= money_reg + 100;
                else if (FiftyCents == 1'b1) 
                    money_reg <= money_reg + 50;
                else if (TenCents == 1'b1)
                    money_reg <= money_reg + 10;
                else if (FiveCents == 1'b1)
                    money_reg <= money_reg + 5;
                if (money_reg >= Price) begin
                    money_reg <= 8'd0;
                    deliver_reg <= 1'b1;
                    State <= READY;
                end
            end
            default: begin
                money_reg <= 8'd0;
                deliver_reg <= 1'b0;
                State <= READY;
            end
        endcase
    end
end
endmodule