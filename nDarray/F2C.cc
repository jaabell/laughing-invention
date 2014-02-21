///*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>


int main(int argc, char *argv[])
{
  if( argc!=4 )
    {
      printf("\a\n Usage:  F2C Fstart_temp Fend_temp  Fstep_temp \n");
      exit(1);
    }

 double start = (double) atof(argv[1]);
 double end   = (double) atof(argv[2]);
 double step  = (double) atof(argv[3]);

 double tempF = 0.0;
 double tempC = 0.0;
 
 ::printf("      F       C  \n ", tempF, tempC);

 for ( tempF = start ; tempF <= end; tempF = tempF + step) 
   {
     tempC = (tempF-32.)*(5.0/9.0);
     ::printf(" %6.1f   %6.1f  \n ", tempF, tempC);
   }

  return 1;
}

