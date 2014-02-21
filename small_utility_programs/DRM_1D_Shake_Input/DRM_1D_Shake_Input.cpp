
///////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (C):     GPL
// PROJECT:           Object Oriented Finite Element Program
// FILE:              TimeHistory_Column_to_Row.cpp
//
// PURPOSE:           Converting the output of Hisada's program to DRM format

// RETURN:
// VERSION:
// LANGUAGE:          C
// TARGET OS:         DOS || UNIX || . . .
// DESIGNER:          Nima Tafazzoli, Boris Jeremic
// PROGRAMMER:        Nima Tafazzoli, Boris Jeremic
// DATE:              October 2010
// UPDATE HISTORY:
///////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <math.h>


using namespace std;

int main( int argc, char* argv[] )
{

    ifstream inputfile;
    ifstream inputfile2;
    ifstream inputfile3;
    ofstream outputfile;
    ofstream outputfile2;


    inputfile.open(argv[1] , ios::in);
    inputfile2.open(argv[2] , ios::in);
    inputfile3.open(argv[3] , ios::in);




    // check number of command-line arguments
    if ( argc != 4 )
    {
        fprintf(stderr, "Usage: DRM_1D_Shake_Input DRM_Nodes_File Displacements_File Acceleration_File \n" );
        exit(1);
    }


    outputfile.open("Displacements.dat" , ios::out);
    outputfile2.open("Accelerations.dat" , ios::out);



    // open input file and check if it exists
    if(! inputfile)
    {
        fprintf(stderr, "Cannot open the input file \n" );
        return 1;
    }

    if(! inputfile2)
    {
        fprintf(stderr, "Cannot open the input2 file \n" );
        return 1;
    }

    if(! inputfile3)
    {
        fprintf(stderr, "Cannot open the input3 file \n" );
        return 1;
    }


    if(! outputfile)
    {
        fprintf(stderr, "Cannot open the output file \n" );
        return 1;
    }

    if(! outputfile2)
    {
        fprintf(stderr, "Cannot open the output2 file \n" );
        return 1;
    }


    int numberofnodes = 0;
    string camma;

    while (!inputfile.eof())
    {
        getline ( inputfile , camma);
        numberofnodes++;
    }

    cout << "\n # of Nodes is : " << numberofnodes << endl;

    inputfile.close();



    double DRM_Nodes[numberofnodes][2];



    inputfile.open(argv[1] , ios::in);

    double nodenumber = 0;
    double Z = 0;

    for (int i = 0; i < numberofnodes; i++)
    {
        inputfile >> nodenumber >> Z;
        DRM_Nodes[i][0] = nodenumber;
        DRM_Nodes[i][1] = Z;
    }


    inputfile.close();



    //     for (int i=0; i< numberofnodes; i++)
    //        {
    //           DRM_Nodes[i][1] = 60 - DRM_Nodes[i][1];
    //        }



    string camma2;
    int numberofsteps = 0;

    while (!inputfile2.eof())
    {
        getline ( inputfile2 , camma2);
        numberofsteps++;
    }

    cout << "\n # of Steps for displacement is : " << numberofsteps << endl;

    inputfile2.close();


    inputfile2.open(argv[2] , ios::in);

    double Displacement[numberofsteps][10];
    double dispcol0, dispcol1, dispcol2, dispcol3, dispcol4, dispcol5, dispcol6, dispcol7, dispcol8, dispcol9;


    for (int i = 0; i < numberofsteps; i++)
    {
        inputfile2 >> dispcol0 >> dispcol1 >> dispcol2 >> dispcol3 >> dispcol4 >> dispcol5 >> dispcol6 >> dispcol7 >> dispcol8 >> dispcol9;
        Displacement[i][0] = dispcol0;
        Displacement[i][1] = dispcol1;
        Displacement[i][2] = dispcol2;
        Displacement[i][3] = dispcol3;
        Displacement[i][4] = dispcol4;
        Displacement[i][5] = dispcol5;
        Displacement[i][6] = dispcol6;
        Displacement[i][7] = dispcol7;
        Displacement[i][8] = dispcol8;
        Displacement[i][9] = dispcol9;
    }

    inputfile2.close();



    string camma3;
    int numberofsteps2 = 0;

    while (!inputfile3.eof())
    {
        getline ( inputfile3 , camma3);
        numberofsteps2++;
    }

    cout << "\n # of Steps for acceleration is : " << numberofsteps2 << endl;

    inputfile3.close();


    inputfile3.open(argv[3] , ios::in);

    double Acceleration[numberofsteps2][10];
    double acccol0, acccol1, acccol2, acccol3, acccol4, acccol5, acccol6, acccol7, acccol8, acccol9;


    for (int i = 0; i < numberofsteps2; i++)
    {
        inputfile3 >> acccol0 >> acccol1 >> acccol2 >> acccol3 >> acccol4 >> acccol5 >> acccol6 >> acccol7 >> acccol8 >> acccol9;
        Acceleration[i][0] = acccol0;
        Acceleration[i][1] = acccol1;
        Acceleration[i][2] = acccol2;
        Acceleration[i][3] = acccol3;
        Acceleration[i][4] = acccol4;
        Acceleration[i][5] = acccol5;
        Acceleration[i][6] = acccol6;
        Acceleration[i][7] = acccol7;
        Acceleration[i][8] = acccol8;
        Acceleration[i][9] = acccol9;
    }


    inputfile3.close();



    int numberofdofs = 3 * numberofnodes;



    double** DisplacementMatrix = new double*[numberofdofs];

    for(int i = 0; i < numberofdofs; ++i)
    {
        DisplacementMatrix[i] = new double[numberofsteps];
    }

    double** AccelerationMatrix = new double*[numberofdofs];

    for(int i = 0; i < numberofdofs; ++i)
    {
        AccelerationMatrix[i] = new double[numberofsteps];
    }


    for (int y = 0; y < numberofdofs; y++)
        for (int z = 0; z < numberofsteps; z++)
        {
            DisplacementMatrix[y][z] = 0.0;
            AccelerationMatrix[y][z] = 0.0;
        }


    int Layer = 0;




    for (int i = 0; i < numberofnodes; i++)
    {


        if(DRM_Nodes[i][1] == 10)
        {
            Layer = 9;
        }

        else if(DRM_Nodes[i][1] == 15)
        {
            Layer = 8;
        }

        else if(DRM_Nodes[i][1] == 20)
        {
            Layer = 7;
        }

        else if(DRM_Nodes[i][1] == 25)
        {
            Layer = 6;
        }

        else if(DRM_Nodes[i][1] == 30)
        {
            Layer = 5;
        }

        else if(DRM_Nodes[i][1] == 35)
        {
            Layer = 4;
        }

        else if(DRM_Nodes[i][1] == 40)
        {
            Layer = 3;
        }

        else if(DRM_Nodes[i][1] == 45)
        {
            Layer = 2;
        }

        else if(DRM_Nodes[i][1] == 50)
        {
            Layer = 1;
        }





        switch (Layer)
        {

            case 1:
                {
                    for (int k = 0; k < numberofsteps; k++)
                    {
                        DisplacementMatrix[3 * i][k] = Displacement[k][1];
                        DisplacementMatrix[3 * i + 1][k] = 0.0;
                        DisplacementMatrix[3 * i + 2][k] = 0.0;
                    }

                    for (int k = 0; k < numberofsteps; k++)
                    {
                        AccelerationMatrix[3 * i][k] = Acceleration[k][1];
                        AccelerationMatrix[3 * i + 1][k] = 0.0;
                        AccelerationMatrix[3 * i + 2][k] = 0.0;
                    }

                    break;
                }


            case 2:
                {

                    for (int k = 0; k < numberofsteps; k++)
                    {
                        DisplacementMatrix[3 * i][k] = Displacement[k][2];
                        DisplacementMatrix[3 * i + 1][k] = 0.0;
                        DisplacementMatrix[3 * i + 2][k] = 0.0;
                    }

                    for (int k = 0; k < numberofsteps; k++)
                    {
                        AccelerationMatrix[3 * i][k] = Acceleration[k][2];
                        AccelerationMatrix[3 * i + 1][k] = 0.0;
                        AccelerationMatrix[3 * i + 2][k] = 0.0;
                    }

                    break;
                }

            case 3:
                {

                    for (int k = 0; k < numberofsteps; k++)
                    {
                        DisplacementMatrix[3 * i][k] = Displacement[k][3];
                        DisplacementMatrix[3 * i + 1][k] = 0.0;
                        DisplacementMatrix[3 * i + 2][k] = 0.0;
                    }

                    for (int k = 0; k < numberofsteps; k++)
                    {
                        AccelerationMatrix[3 * i][k] = Acceleration[k][3];
                        AccelerationMatrix[3 * i + 1][k] = 0.0;
                        AccelerationMatrix[3 * i + 2][k] = 0.0;
                    }

                    break;
                }

            case 4:
                {

                    for (int k = 0; k < numberofsteps; k++)
                    {
                        DisplacementMatrix[3 * i][k] = Displacement[k][4];
                        DisplacementMatrix[3 * i + 1][k] = 0.0;
                        DisplacementMatrix[3 * i + 2][k] = 0.0;
                    }

                    for (int k = 0; k < numberofsteps; k++)
                    {
                        AccelerationMatrix[3 * i][k] = Acceleration[k][4];
                        AccelerationMatrix[3 * i + 1][k] = 0.0;
                        AccelerationMatrix[3 * i + 2][k] = 0.0;
                    }

                    break;
                }

            case 5:
                {

                    for (int k = 0; k < numberofsteps; k++)
                    {
                        DisplacementMatrix[3 * i][k] = Displacement[k][5];
                        DisplacementMatrix[3 * i + 1][k] = 0.0;
                        DisplacementMatrix[3 * i + 2][k] = 0.0;
                    }

                    for (int k = 0; k < numberofsteps; k++)
                    {
                        AccelerationMatrix[3 * i][k] = Acceleration[k][5];
                        AccelerationMatrix[3 * i + 1][k] = 0.0;
                        AccelerationMatrix[3 * i + 2][k] = 0.0;
                    }

                    break;
                }

            case 6:
                {

                    for (int k = 0; k < numberofsteps; k++)
                    {
                        DisplacementMatrix[3 * i][k] = Displacement[k][6];
                        DisplacementMatrix[3 * i + 1][k] = 0.0;
                        DisplacementMatrix[3 * i + 2][k] = 0.0;
                    }

                    for (int k = 0; k < numberofsteps; k++)
                    {
                        AccelerationMatrix[3 * i][k] = Acceleration[k][6];
                        AccelerationMatrix[3 * i + 1][k] = 0.0;
                        AccelerationMatrix[3 * i + 2][k] = 0.0;
                    }

                    break;
                }

            case 7:
                {

                    for (int k = 0; k < numberofsteps; k++)
                    {
                        DisplacementMatrix[3 * i][k] = Displacement[k][7];
                        DisplacementMatrix[3 * i + 1][k] = 0.0;
                        DisplacementMatrix[3 * i + 2][k] = 0.0;
                    }

                    for (int k = 0; k < numberofsteps; k++)
                    {
                        AccelerationMatrix[3 * i][k] = Acceleration[k][7];
                        AccelerationMatrix[3 * i + 1][k] = 0.0;
                        AccelerationMatrix[3 * i + 2][k] = 0.0;
                    }

                    break;
                }

            case 8:
                {

                    for (int k = 0; k < numberofsteps; k++)
                    {
                        DisplacementMatrix[3 * i][k] = Displacement[k][8];
                        DisplacementMatrix[3 * i + 1][k] = 0.0;
                        DisplacementMatrix[3 * i + 2][k] = 0.0;
                    }

                    for (int k = 0; k < numberofsteps; k++)
                    {
                        AccelerationMatrix[3 * i][k] = Acceleration[k][8];
                        AccelerationMatrix[3 * i + 1][k] = 0.0;
                        AccelerationMatrix[3 * i + 2][k] = 0.0;
                    }

                    break;
                }

            case 9:
                {

                    for (int k = 0; k < numberofsteps; k++)
                    {
                        DisplacementMatrix[3 * i][k] = Displacement[k][9];
                        DisplacementMatrix[3 * i + 1][k] = 0.0;
                        DisplacementMatrix[3 * i + 2][k] = 0.0;
                    }

                    for (int k = 0; k < numberofsteps; k++)
                    {
                        AccelerationMatrix[3 * i][k] = Acceleration[k][9];
                        AccelerationMatrix[3 * i + 1][k] = 0.0;
                        AccelerationMatrix[3 * i + 2][k] = 0.0;
                    }

                    break;
                }




            default:
                return -1;
                break;


        }
    }


    for( int i = 0; i < numberofdofs; i++)
    {
        for (int j = 0; j < numberofsteps; j++)
        {
            outputfile << DisplacementMatrix[i][j] << " ";
        }

        outputfile << endl;
    }



    for( int i = 0; i < numberofdofs; i++)
    {
        for (int j = 0; j < numberofsteps; j++)
        {
            outputfile2 << AccelerationMatrix[i][j] << " ";
        }

        outputfile2 << endl;
    }

    cout << "\n *** The end, enjoy the rest of the work! *** \n" << endl;

    return 0;
}



