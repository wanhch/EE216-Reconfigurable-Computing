#define N 16
typedef int coef_t;
typedef int data_t;
typedef int acc_t;
void fir(data_t *y, data_t x) { 
    coef_t c[N] = {1,-2,3,-4,5,-6,7,-8,-8,7,-6,5,-4,3,-2,1};
    static data_t shift_reg[N];
    acc_t acc;
    int i;
    acc = 0;
    fir_label0:for (i = N - 1; i >= 0; i--) {
        if (i == 0) {
            acc += x * c[0];
            shift_reg[0] = x;
        } else {
            shift_reg[i] = shift_reg[i - 1];
            acc += shift_reg[i] * c[i];
        }
    }
    *y = acc;
}
