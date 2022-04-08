module FA (
    input  ci,
    input  A ,
    input  B ,
    output S ,
    output co);

    assign {co, S} = A + B + ci;
endmodule

module FA8 (
    input  [7:0] A,
    input  [7:0] B,
    input  ci     ,
    output co     ,
    output [7:0] S);

    wire [7:0] P;
    wire [7:0] C;
    wire BP;
    assign P[0] = A[0] ^ B[0];
    assign P[1] = A[1] ^ B[1];
    assign P[2] = A[2] ^ B[2];
    assign P[3] = A[3] ^ B[3];
    assign P[4] = A[4] ^ B[4];
    assign P[5] = A[5] ^ B[5];
    assign P[6] = A[6] ^ B[6];
    assign P[7] = A[7] ^ B[7];
    assign BP = P[0] & P[1] & P[2] & P[3] & P[4] & P[5] & P[6] & P[7];
    assign co = BP ? ci : C[7];

    FA FA_0(ci, A[0], B[0], S[0], C[0]);
    FA FA_1(C[0], A[1], B[1], S[1], C[1]);
    FA FA_2(C[1], A[2], B[2], S[2], C[2]);
    FA FA_3(C[2], A[3], B[3], S[3], C[3]);
    FA FA_4(C[3], A[4], B[4], S[4], C[4]);
    FA FA_5(C[4], A[5], B[5], S[5], C[5]);
    FA FA_6(C[5], A[6], B[6], S[6], C[6]);
    FA FA_7(C[6], A[7], B[7], S[7], C[7]);
endmodule

module MultiCycleAdder (
    input  [15:0] A,
    input  [15:0] B,
    input  ci,
    input  clk,
    input  rst,
    output [15:0] S);

    reg [7:0] buffer_A;
    reg [7:0] buffer_B;
    reg [7:0] buffer_sumL;
    reg buffer_carry;

    wire [7:0] sumL_sig_in;
    wire [7:0] sumL_sig_out;
    wire [7:0] sumH_sig;
    wire carry_sig_in;
    wire carry_sig_out;
    

    assign S = {sumH_sig, sumL_sig_out};
    assign sumL_sig_out = buffer_sumL;
    assign carry_sig_out = buffer_carry;

    FA8 FA8H(buffer_A, buffer_B, carry_sig_out, , sumH_sig);
    FA8 FA8L(A[7:0], B[7:0], ci, carry_sig_in, sumL_sig_in);
    always @(posedge clk or posedge rst) begin
        
        if (rst == 1'b0) begin
            buffer_A <= 8'd0;
            buffer_B <= 8'd0;
            buffer_carry <= 1'b0;
            buffer_sumL <= 8'd0;
        end else begin
            buffer_A <= A[15:8];
            buffer_B <= B[15:8];
            buffer_carry <= carry_sig_in;
            buffer_sumL <= sumL_sig_in;
        end
    end
endmodule