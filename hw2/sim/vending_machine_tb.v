module tb_vending_machine;
    reg  Enable;
    reg  RST;
    reg  CLK;
    reg  OneDollar;
    reg  FiftyCents;
    reg  TenCents;
    reg  FiveCents;
    wire ButtonOut;
    wire [7:0] Money;
    
    vending_machine dut(Enable,RST,CLK, OneDollar,FiftyCents,TenCents,FiveCents,ButtonOut,Money);
    
    always #10 CLK = ~CLK;
    
    initial begin
        Enable = 0;
        CLK = 0;
        RST = 0;
        OneDollar = 0;
        FiftyCents = 0;
        TenCents = 0;
        FiveCents = 0;
        #15;
        RST = 1;
        OneD();
        #10
        TenC();
        #10
        En();
        #10
        OneD();
        #10
        TenC();
        #10
        TenC();
        #10
        FiveC();
        #10
        #10
        #10
        #10
        $finish;
    end
    
    task En();
        begin
            Enable = 1;
            @(posedge CLK);
            #5 Enable = 0;
        end
    endtask

    task OneD();
        begin
            OneDollar = 1;
            @(posedge CLK);
            #5 OneDollar = 0;
        end
    endtask
    
    task FiftyC();
        begin
            FiftyCents = 1;
            @(posedge CLK);
            #5 FiftyCents = 0;
        end
    endtask

    task TenC();
        begin
            TenCents = 1;
            @(posedge CLK);
            #5 TenCents = 0;
        end
    endtask

    task FiveC();
        begin
            FiveCents = 1;
            @(posedge CLK);
            #5 FiveCents = 0;
        end
    endtask
    
    
endmodule 
