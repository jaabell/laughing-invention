///////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (C):     GPL
// PROJECT:           Object Oriented Finite Element Program
// FILE:              TimeHistory_Column_to_Row.cpp
//
// PURPOSE:           convert the column-wise time history data to row-wise

// RETURN:
// VERSION:
// LANGUAGE:          C
// TARGET OS:         DOS || UNIX || . . .
// DESIGNER:          Nima Tafazzoli
// PROGRAMMER:        Nima Tafazzoli
// DATE:              01 June 2010
// UPDATE HISTORY:
///////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <fstream>
#include <stdio.h>
using namespace std;

int main( int argc, char* argv[] )
{

    ifstream inputfile; // input file with column-wise data
    ofstream outputfile; // output file with row-wise data


    inputfile.open(argv[ 1 ] , ios::in);
    outputfile.open(argv[ 2 ] , ios::out);


    // check number of command-line arguments
    if ( argc != 3 )
    {
        fprintf(stderr, "Usage: TimeHistory_Column_to_Row input_column_wise_file  output_row_wise_file \n" );
        exit(1);
    }


    double data;

    while (!inputfile.eof())
    {
        inputfile >> data;
        outputfile << data << " ";
    }

    return 0;
}



