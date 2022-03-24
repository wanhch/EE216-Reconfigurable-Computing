module tb_debouncing;
    reg  CLK;
    reg  ButtonIn;
    wire ButtonOut;
    
    debouncing dut(CLK, ButtonIn, ButtonOut);
    
    always #10 CLK = ~CLK;
    
    initial begin
        CLK = 0;
        ButtonIn = 0;
        #15;
        ButtonIn = 1;
        #20;
        ButtonIn = 0;
        #50
        ButtonIn = 1;
        #100
        ButtonIn = 0;
        #50
        // trig(21);
        // idle(4);
        // trig(3);
        // idle(5);
        $finish;
    end
    
    // task trig (delay);
    // begin
    //         ButtonIn = 1;
    //         @(posedge CLK);
    //         #delay ButtonIn = 0;
    //     end
    // endtask
    
    // task idle(delay);
    // begin
    //         ButtonIn = 0;
    //         @(posedge CLK);
    //         #delay ButtonIn = 0;
    // end
    // endtask
    
    
endmodule 
