#include <stdio.h>
#include <math.h>
#include "FIR.h"

int main () {
  const int    SAMPLES=20;
  FILE         *fp;

  data_t signal, output;

  int i;
  signal = 0;
  
  fp=fopen("out.dat", "w");
  for (i=0; i<=SAMPLES; i++) {
    if (i == 0) 
        signal = 1;
    else 
        signal = 0;

    fir(&output, signal);

    fprintf(fp,"%i %d %d\n",i,signal,output);
  }
  fclose(fp);
  
  printf ("Comparing against output data \n");
  if (system("diff -w out.dat out.gold.dat")) {

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
