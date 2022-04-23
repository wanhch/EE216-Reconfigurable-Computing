#include "headseeker.h"

void headseeker(hls::stream<ap_axiu<32,0,0,0> >& msg_in, hls::stream<ap_axiu<32,0,0,0> >& msg_out) {
    #pragma HLS pipeline II=1

    ap_axiu<32,0,0,0> curr_word;
    ap_axiu<32,0,0,0> send_word;
    ap_uint<32> msg;
    ap_uint<32> out;
    ap_uint<8> posi;
    bool find_flag;

    static enum State {FRIST_IN = 0, FIND, SPLIT, DIR_OUT, DONE, WAIT} HS_State;
    ap_uint<8> buff[8];

    if(!msg_in.empty()) {
        msg_in.read(curr_word);
        msg = curr_word.data;
    }

        switch(HS_State) {
            case FRIST_IN:
                buff[4] = msg.range(31, 24);
                buff[5] = msg.range(23, 16);
                buff[6] = msg.range(15, 8);
                buff[7] = msg.range(7, 0);
                for (int i = 0; i < 4; ++i) {
                    out.range(31-8*i, 24-8*i) = (int)(' ');
                }
                find_flag = false;
                HS_State = FIND;
            break;

            case FIND:
                for (int i = 0; i < 4; ++i) {
                    buff[i] = buff[i + 4];
                }
                buff[4] = msg.range(31, 24);
                buff[5] = msg.range(23, 16);
                buff[6] = msg.range(15, 8);
                buff[7] = msg.range(7, 0);
                for (int i = 0; i < 4; ++i) {
                    if (((char)buff[i] == 'E') && ((char)buff[i+1] == 'E') && ((char)buff[i+2] == '2') && ((char)buff[i+3] == '1') && ((char)buff[i+4] == '6')) {
                        find_flag = true;
                        posi = i;
                        HS_State = SPLIT;
                        // break;
                    }
                }
                if (find_flag == false || posi == 0) {
                    posi = 4;
                }
                for (int i = 0; i < 4; ++i) {
                    if (i <= posi - 1) {
                        out.range(31-8*i, 24-8*i) = buff[i];
                    } else {
                        out.range(31-8*i, 24-8*i) = (int)(' ');
                    }
                }
            break;

            case SPLIT:
                for (int i = 0; i < 4; ++i) {
                    out.range(31-8*i, 24-8*i) = buff[i + posi];
                }
                if (posi == 4) {
                    buff[0] = msg.range(31, 24);
                    buff[1] = msg.range(23, 16);
                    buff[2] = msg.range(15, 8);
                    buff[3] = msg.range(7, 0);
                } else {
                    for (int i = 0; i < 4 - posi; ++i) {
                        buff[i] = buff[4+posi+i];
                    }
                    buff[4-posi] = msg.range(31, 24);
                    buff[4-posi+1] = msg.range(23, 16);
                    buff[4-posi+2] = msg.range(15, 8);
                    buff[4-posi+3] = msg.range(7, 0);
                }
                HS_State = DIR_OUT;
            break;

            case DIR_OUT:
                
                if (posi == 4) {
                    for (int i = 0; i < 4; ++i) {
                        out.range(31-8*i, 24-8*i) = buff[i];
                    }
                    buff[0] = msg.range(31, 24);
                    buff[1] = msg.range(23, 16);
                    buff[2] = msg.range(15, 8);
                    buff[3] = msg.range(7, 0);
                } else {
                    for (int i = 0; i < 4; ++i) {
                        out.range(31-8*i, 24-8*i) = buff[i];
                    }
                    for (int i = 0; i < 4 - posi; ++i) {
                        buff[i] = buff[4+i];
                    }
                    buff[4-posi] = msg.range(31, 24);
                    buff[4-posi+1] = msg.range(23, 16);
                    buff[4-posi+2] = msg.range(15, 8);
                    buff[4-posi+3] = msg.range(7, 0);
                }
                if (curr_word.last) {
                    HS_State = DONE;
                }
            break;

            case DONE:
                for (int i = 0; i < 4; ++i) {
                    out.range(31-8*i, 24-8*i) = buff[i];

                }
                if (posi == 4) {
                    HS_State = FRIST_IN;
                }
                else {
                    for (int i = 0; i < 4 - posi; ++i) {
                        buff[i] = buff[4+i];
                    }
                    buff[4-posi] = msg.range(31, 24);
                    buff[4-posi+1] = msg.range(23, 16);
                    buff[4-posi+2] = msg.range(15, 8);
                    buff[4-posi+3] = msg.range(7, 0);
                    HS_State = WAIT;
                }
            break;

            case WAIT:
                for (int i = 0; i < 4; ++i) {
                    if (i < 4 - posi) {
                        out.range(31-8*i, 24-8*i) = buff[i];
                    } else {
                        out.range(31-8*i, 24-8*i) = (int)(' ');
                    }
                }
                buff[4] = msg.range(31, 24);
                buff[5] = msg.range(23, 16);
                buff[6] = msg.range(15, 8);
                buff[7] = msg.range(7, 0);
                find_flag = false;
                HS_State = FIND;
        }
    send_word.data = out;
    msg_out.write(send_word);
}
