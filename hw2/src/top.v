module top (
    input  Enable,
    input  RST,
    input  CLK,
    input  OneDollar,
    input  FiftyCents,
    input  TenCents,
    input  FiveCents,
    output Deliver,
    output [7:0] Money);

wire de_OneDollar;
wire de_FiftyCents;
wire de_TenCents;
wire de_FiveCents;

debouncing u_de_OneDollar (
    .CLK       (CLK         ), 
    .ButtonIn  (OneDollar   ),
    .ButtonOut (de_OneDollar));
debouncing u_de_FiftyCents (
    .CLK       (CLK          ), 
    .ButtonIn  (FiftyCents   ),
    .ButtonOut (de_FiftyCents));
debouncing u_de_TenCents (
    .CLK       (CLK        ), 
    .ButtonIn  (TenCents   ),
    .ButtonOut (de_TenCents));
debouncing u_de_FiveCents (
    .CLK       (CLK         ), 
    .ButtonIn  (FiveCents   ),
    .ButtonOut (de_FiveCents));
vending_machine FSM (
    .Enable     (Enable       ),
    .RST        (RST          ),
    .CLK        (CLK          ),
    .OneDollar  (de_OneDollar ),
    .FiftyCents (de_FiftyCents),
    .TenCents   (de_TenCents  ),
    .FiveCents  (de_FiveCents ),
    .Deliver    (Deliver      ),
    .Money      (Money        ));
endmodule