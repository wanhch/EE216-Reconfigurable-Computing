module tb_MultiCycleAdder;
    reg  ci;
    reg  [15:0] A;
    reg  [15:0] B;
    reg clk;
    reg rst;
    wire signed [15:0] S;
    
    MultiCycleAdder dut(A, B, ci, clk, rst, S);
    
    always #10 clk = ~clk;
    
    initial begin
        clk = 0;
        rst = 0;
        #10
        #10
        ci = 0;
        rst = 1;
        A = 101;
        B = 113;
        #20
        #20
        #20
        $finish;
    end
    

    
endmodule 
