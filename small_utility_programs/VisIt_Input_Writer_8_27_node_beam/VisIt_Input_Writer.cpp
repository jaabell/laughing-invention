///////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (C):     GPL
// PROJECT:           Object Oriented Finite Element Program
//
// PURPOSE:           Creating Input for VisIt Visualization Program
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:         DOS || UNIX || . . .
// DESIGNER:          Nima Tafazzoli, Boris Jeremic
// PROGRAMMER:        Nima Tafazzoli, Boris Jeremic
// DATE:              September 2010
// UPDATE HISTORY:
///////////////////////////////////////////////////////////////////////////////

// make sure your input files do not have blank line at the end


#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <sstream>


using namespace std;


int main( int argc, char* argv[] )
{


    // ************************
    // Reading the nodes for eightnodebrick elements and beam elements
    // and write them in desired format for VisIt


    int Number_of_27NodeBrick_Nodes = 0;


    const char* twentysevennodebricknodesfilename;


    ifstream twentysevennodebricknodesinputfile;

    cout << "\n*************************************************************************";
    cout << "\n*** Make sure the input files do not have blank line at the end!";
    cout << "\n*** Combine all the files containing EightNodeBrick nodes in one file";
    cout << "\n*** Combine all the files containing Beam nodes in one file";
    cout << "\n*************************************************************************\n";


    string twentysevennodebricknodesfilenamestr;
    cout << "\n Enter the file name with 27 node brick nodes: ";
    getline(cin, twentysevennodebricknodesfilenamestr);



    // ************************
    // Check if there is any data for twentysevennodebrick nodes
    // and count number of the nodes

    if (!twentysevennodebricknodesfilenamestr.empty())
    {


        twentysevennodebricknodesfilename = twentysevennodebricknodesfilenamestr.c_str();

        twentysevennodebricknodesinputfile.open(twentysevennodebricknodesfilename , ios::in);


        if(! twentysevennodebricknodesinputfile)
        {
            fprintf(stderr, "\n Cannot open the twentysevennodebrick file with nodal coordinates in it \n" );
            return 1;
        }



        string camma;

        while (!twentysevennodebricknodesinputfile.eof())
        {
            getline ( twentysevennodebricknodesinputfile , camma);
            Number_of_27NodeBrick_Nodes++;
        }

        cout << "\n # of TwentySeven_Node_Brick Nodes is : " << Number_of_27NodeBrick_Nodes << endl;

        twentysevennodebricknodesinputfile.close();

    }



    // ************************

    int total_number_of_nodes = Number_of_27NodeBrick_Nodes;
    //   int ndofs_soil = 3 * Number_of_27NodeBrick_Nodes;


    float FEM_Nodes[total_number_of_nodes][5];


    for (int i = 0; i < total_number_of_nodes; i++)
    {
        FEM_Nodes[i][0] = i;
    }


    // ************************
    // Check if there is any data for soil nodes
    // and write the nodes to a two-dimensional array
    // called FEM_Nodes


    if ( Number_of_27NodeBrick_Nodes != 0)
    {

        twentysevennodebricknodesinputfile.open(twentysevennodebricknodesfilename , ios::in);


        if(! twentysevennodebricknodesinputfile)
        {
            fprintf(stderr, "\n Cannot open the twentysevennodebricknodesinputfile file \n" );
            return 1;
        }


        //      float dataPoint = 0;
        int nodenumber = 0;



        for (int i = 0; i < Number_of_27NodeBrick_Nodes; i++)
        {

            twentysevennodebricknodesinputfile >> nodenumber;
            FEM_Nodes[i][1] = nodenumber;


            for (int j = 2; j < 5; j++)
            {
                twentysevennodebricknodesinputfile >> FEM_Nodes[i][j];
            }
        }

        twentysevennodebricknodesinputfile.close();

    }



    // ***********************************************************************************
    // Check if there is any data for eightnodebrick elements
    // and count number of the elements


    int Number_of_27NodeBrick_Elements = 0;


    const char* twentysevennodebrickelementsfilename;

    ifstream twentysevennodebrickelementsinputfile;


    string twentysevennodebrickelementsfilenamestr;
    cout << "\n Enter the file name with soil elements: ";
    getline(cin, twentysevennodebrickelementsfilenamestr);



    if (!twentysevennodebrickelementsfilenamestr.empty())
    {

        twentysevennodebrickelementsfilename = twentysevennodebrickelementsfilenamestr.c_str();

        twentysevennodebrickelementsinputfile.open(twentysevennodebrickelementsfilename , ios::in);


        if(! twentysevennodebrickelementsinputfile)
        {
            fprintf(stderr, "\n Cannot open the twentysevennodebrickelementsinputfile file \n" );
            return 1;
        }


        string camma;

        while (!twentysevennodebrickelementsinputfile.eof())
        {
            getline ( twentysevennodebrickelementsinputfile , camma);
            Number_of_27NodeBrick_Elements++;
        }

        cout << "\n # of 27NodeBrick Elements is : " << Number_of_27NodeBrick_Elements << endl;

        twentysevennodebrickelementsinputfile.close();

    }



    // ************************************

    int total_number_of_elements = Number_of_27NodeBrick_Elements;

    int celltypes[total_number_of_elements];
    int nconnectivity_27nodebrick = 27 * Number_of_27NodeBrick_Elements;
    int connectivity_27nodebrick[nconnectivity_27nodebrick];
    int matrixconnectivity_27nodebrick[Number_of_27NodeBrick_Elements][27];
    int updatedconnectivity_27nodebrick[Number_of_27NodeBrick_Elements][27];

    // array for storing the material number for each element
    int Element_Materials[total_number_of_elements];


    // **************************************************
    // Check if there is any data for soil elements
    // and write the elements into one-dimensional array
    // called connectivity_8nodebrick_temp which just knows about the
    // connectivities


    if ( Number_of_27NodeBrick_Elements != 0)
    {

        for (int i = 0; i < Number_of_27NodeBrick_Elements; i++)
        {
            celltypes[i] = 29;
        }


        twentysevennodebrickelementsinputfile.open(twentysevennodebrickelementsfilename , ios::in);


        if(! twentysevennodebrickelementsinputfile)
        {
            fprintf(stderr, "\n Cannot open the twentysevennodebrickelementsinputfile file \n" );
            return 1;
        }


        int dataPoint2 = 0;
        int cols2 = 27;

        double connectivity_27nodebrick_temp[nconnectivity_27nodebrick];


        for (int i = 0; i < Number_of_27NodeBrick_Elements; i++)
        {

            twentysevennodebrickelementsinputfile >> dataPoint2;

            for (int j = 0; j < cols2; j++)
            {
                int k = i * 27 + j;
                twentysevennodebrickelementsinputfile >> connectivity_27nodebrick_temp[k];
            }

            twentysevennodebrickelementsinputfile >> Element_Materials[i];
        }

        twentysevennodebrickelementsinputfile.close();


        // **************************************************
        // connectivity nodes should start from zero and should be from
        // zero to (Number_of_EightNodeBrick_Elements-1)
        // Here connectivity_8nodebrick_temp components which compare to Node Numbers
        // which are from zero to (Number_of_EightNodeBrick_Elements-1) and write
        // them into new one-dimensional array called conn_soil

        for (int i = 0; i < nconnectivity_27nodebrick; i++)
        {
            for (int j = 0; j < total_number_of_nodes; j++)
            {
                if (connectivity_27nodebrick_temp[i] == FEM_Nodes[j][1])
                {
                    connectivity_27nodebrick[i] = FEM_Nodes[j][0];
                    break;
                }
            }
        }

    }

    for (int m = 0; m < Number_of_27NodeBrick_Elements; m++)
    {
        for (int n = 0; n < 27; n++)
        {
            matrixconnectivity_27nodebrick[m][n] = connectivity_27nodebrick[m * 27 + n];
        }
    }


    for (int m = 0; m < Number_of_27NodeBrick_Elements; m++)
    {
        updatedconnectivity_27nodebrick[m][0]  = matrixconnectivity_27nodebrick[m][7];
        updatedconnectivity_27nodebrick[m][1]  = matrixconnectivity_27nodebrick[m][4];
        updatedconnectivity_27nodebrick[m][2]  = matrixconnectivity_27nodebrick[m][5];
        updatedconnectivity_27nodebrick[m][3]  = matrixconnectivity_27nodebrick[m][6];
        updatedconnectivity_27nodebrick[m][4]  = matrixconnectivity_27nodebrick[m][3];
        updatedconnectivity_27nodebrick[m][5]  = matrixconnectivity_27nodebrick[m][0];
        updatedconnectivity_27nodebrick[m][6]  = matrixconnectivity_27nodebrick[m][1];
        updatedconnectivity_27nodebrick[m][7]  = matrixconnectivity_27nodebrick[m][2];
        updatedconnectivity_27nodebrick[m][8]  = matrixconnectivity_27nodebrick[m][15];
        updatedconnectivity_27nodebrick[m][9]  = matrixconnectivity_27nodebrick[m][12];
        updatedconnectivity_27nodebrick[m][10] = matrixconnectivity_27nodebrick[m][13];
        updatedconnectivity_27nodebrick[m][11] = matrixconnectivity_27nodebrick[m][14];
        updatedconnectivity_27nodebrick[m][12] = matrixconnectivity_27nodebrick[m][11];
        updatedconnectivity_27nodebrick[m][13] = matrixconnectivity_27nodebrick[m][8];
        updatedconnectivity_27nodebrick[m][14] = matrixconnectivity_27nodebrick[m][9];
        updatedconnectivity_27nodebrick[m][15] = matrixconnectivity_27nodebrick[m][10];
        updatedconnectivity_27nodebrick[m][16] = matrixconnectivity_27nodebrick[m][19];
        updatedconnectivity_27nodebrick[m][17] = matrixconnectivity_27nodebrick[m][16];
        updatedconnectivity_27nodebrick[m][18] = matrixconnectivity_27nodebrick[m][17];
        updatedconnectivity_27nodebrick[m][19] = matrixconnectivity_27nodebrick[m][18];
        updatedconnectivity_27nodebrick[m][20] = matrixconnectivity_27nodebrick[m][24];
        updatedconnectivity_27nodebrick[m][21] = matrixconnectivity_27nodebrick[m][21];
        updatedconnectivity_27nodebrick[m][22] = matrixconnectivity_27nodebrick[m][22];
        updatedconnectivity_27nodebrick[m][23] = matrixconnectivity_27nodebrick[m][23];
        updatedconnectivity_27nodebrick[m][24] = matrixconnectivity_27nodebrick[m][26];
        updatedconnectivity_27nodebrick[m][25] = matrixconnectivity_27nodebrick[m][25];
        updatedconnectivity_27nodebrick[m][26] = matrixconnectivity_27nodebrick[m][20];
    }

    //**********************************************************************


    ////////////////////////////////////////
    // converting integer to string
    string integertostring;
    stringstream out;
    out << 1;
    integertostring = out.str();

    ////////////////////////////////////////
    // combining "time" +  i (as string) with ".vtk" and convert it to char
    // such that we can use it for outputfile stream

    string filenamestring = "time" + integertostring + ".vtk";

    const char* filename = filenamestring.c_str();

    ofstream outputfile;
    outputfile.open(filename , ios::out);


    outputfile << "# vtk DataFile Version 2.0" << endl;
    outputfile << "Written using VisIt writer" << endl;
    outputfile << "ASCII" << endl;
    outputfile << "DATASET UNSTRUCTURED_GRID" << endl;
    outputfile << "POINTS " << total_number_of_nodes << " float" << endl;

    for (int k = 0; k < total_number_of_nodes; k++)
    {
        outputfile << FEM_Nodes[k][2] << " " << FEM_Nodes[k][3] << " " << FEM_Nodes[k][4] << endl;
    }


    outputfile << "CELLS " << total_number_of_elements << " " << Number_of_27NodeBrick_Elements * 28 << endl;


    for (int m = 0; m < Number_of_27NodeBrick_Elements; m++)
    {
        outputfile << "27" << "  ";

        for (int n = 0; n < 27; n++)
        {
            outputfile << updatedconnectivity_27nodebrick[m][n] << "  ";
        }

        outputfile << endl;
    }



    outputfile << "CELL_TYPES " << total_number_of_elements << endl;

    for (int x = 0; x < Number_of_27NodeBrick_Elements; x++)
    {
        outputfile << celltypes[x] << endl;
    }



    outputfile << "CELL_DATA " << total_number_of_elements << endl;
    outputfile << "SCALARS Materials int 1" << endl;
    outputfile << "LOOKUP_TABLE default" << endl;

    for (int s = 0; s < total_number_of_elements; s++)
    {
        outputfile << Element_Materials[s] << endl;
    }

    ///////////////////////////////////////////////////////////////////


    return 0;
}



