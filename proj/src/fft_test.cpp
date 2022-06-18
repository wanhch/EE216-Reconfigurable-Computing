
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fft.h"

 #define __SDSOC_COMPILE

#ifdef __SDSOC_COMPILE
#include "sds_lib.h"
#else
#define sds_alloc malloc
#define sds_free free
#endif

struct Rmse
{
    int num_sq;
    float sum_sq;
    float error;
    
    Rmse(){ num_sq = 0; sum_sq = 0; error = 0; }
    
    float add_value(float d_n)
    {
        num_sq++;
        sum_sq += (d_n*d_n);
        error = sqrtf(sum_sq / num_sq);
        return error;
    }
    
};


Rmse rmse_R,  rmse_I;

int main()
{
    int index;
    DTYPE gold_R, gold_I;
    timeval start,end;


    FILE * fp = fopen("out.gold.dat","r");

    DTYPE* In_R = (DTYPE*)sds_alloc(SIZE*sizeof(DTYPE));
    DTYPE* In_I = (DTYPE*)sds_alloc(SIZE*sizeof(DTYPE));
    DTYPE* Out_R = (DTYPE*)sds_alloc(SIZE*sizeof(DTYPE)); 
    DTYPE* Out_I = (DTYPE*)sds_alloc(SIZE*sizeof(DTYPE));

	//Generate input data
	for(int i=0; i<SIZE; i++)	{
		In_R[i] = i;
		In_I[i] = 0.0;
		Out_R[i]=i;
		Out_I[i]=1;
	}

    printf("This is EE216 FFT Project Testbench\n");

	//Perform FFT
    gettimeofday(&start, NULL);
	fft_hw(In_R, In_I, Out_R, Out_I);
    gettimeofday(&end, NULL);
    printf("fft_hw %lu us\n",(end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec));

    // comparing with golden output
    for(int i=0; i<SIZE; i++)
    {
        fscanf(fp, "%d %f %f", &index, &gold_R, &gold_I);
        rmse_R.add_value(Out_R[i] - gold_R);
        rmse_I.add_value(Out_I[i] - gold_I);
    }
    fclose(fp);
    sds_free(In_R);
    sds_free(In_I);
    sds_free(Out_R);
    sds_free(Out_I);


    // printing error results
    printf("----------------------------------------------\n");
    printf("   RMSE(R)           RMSE(I)\n");
    printf("%0.15f %0.15f\n", rmse_R.error, rmse_I.error);
    printf("----------------------------------------------\n");
    
    if (rmse_R.error > 1 || rmse_I.error > 1 ) {
        fprintf(stdout, "*******************************************\n");
        fprintf(stdout, "FAIL: Output DOES NOT match the golden output\n");
        fprintf(stdout, "*******************************************\n");
        return 1;
    } else {
        fprintf(stdout, "*******************************************\n");
        fprintf(stdout, "PASS: The output matches the golden output!\n");
        fprintf(stdout, "*******************************************\n");
        return 0;
    }
}
