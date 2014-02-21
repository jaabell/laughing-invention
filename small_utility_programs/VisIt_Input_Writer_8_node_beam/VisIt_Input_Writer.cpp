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


    int Number_of_EightNodeBrick_Nodes = 0;
    int Number_of_Beam_Nodes = 0;


    const char* eightnodebricknodesfilename;
    const char* beamnodesfilename;


    ifstream eightnodebricknodesinputfile;
    ifstream beamnodesinputfile;

    cout << "\n*************************************************************************";
    cout << "\n*** Make sure the input files do not have blank line at the end!";
    cout << "\n*** Combine all the files containing EightNodeBrick nodes in one file";
    cout << "\n*** Combine all the files containing Beam nodes in one file";
    cout << "\n*************************************************************************\n";


    string eightnodebricknodesfilenamestr;
    cout << "\n Enter the file name with soil nodes: ";
    getline(cin, eightnodebricknodesfilenamestr);


    string beamnodesfilenamestr;
    cout << "\n Enter the file name with beam nodes: ";
    getline( cin, beamnodesfilenamestr);



    // ************************
    // Check if there is any data for eightnodebrick nodes
    // and count number of the nodes

    if (!eightnodebricknodesfilenamestr.empty())
    {


        eightnodebricknodesfilename = eightnodebricknodesfilenamestr.c_str();

        eightnodebricknodesinputfile.open(eightnodebricknodesfilename , ios::in);


        if(! eightnodebricknodesinputfile)
        {
            fprintf(stderr, "\n Cannot open the eightnodebrick file with nodal coordinates in it \n" );
            return 1;
        }



        string camma;

        while (!eightnodebricknodesinputfile.eof())
        {
            getline ( eightnodebricknodesinputfile , camma);
            Number_of_EightNodeBrick_Nodes++;
        }

        cout << "\n # of Eight_Node_Brick Nodes is : " << Number_of_EightNodeBrick_Nodes << endl;

        eightnodebricknodesinputfile.close();

    }


    // ************************
    // Check if there is any data for beam nodes
    // and count number of the nodes


    if (!beamnodesfilenamestr.empty())
    {

        beamnodesfilename = beamnodesfilenamestr.c_str();

        beamnodesinputfile.open(beamnodesfilename , ios::in);


        if(! beamnodesinputfile)
        {
            fprintf(stderr, "\n Cannot open the beamnodesinputfile file \n" );
            return 1;
        }


        string camma;

        while (!beamnodesinputfile.eof())
        {
            getline ( beamnodesinputfile , camma);
            Number_of_Beam_Nodes++;
        }

        cout << "\n # of Beam Nodes is : " << Number_of_Beam_Nodes << endl;

        beamnodesinputfile.close();

    }



    // ************************

    int total_number_of_nodes = Number_of_EightNodeBrick_Nodes + Number_of_Beam_Nodes;
    //  int ndofs_beam = 6 * Number_of_Beam_Nodes;
    int ndofs_beam = 3 * Number_of_Beam_Nodes;
    int ndofs_soil = 3 * Number_of_EightNodeBrick_Nodes;


    int FEM_Nodes[total_number_of_nodes][5];


    for (int i = 0; i < total_number_of_nodes; i++)
    {
        FEM_Nodes[i][0] = i;
    }


    // ************************
    // Check if there is any data for soil nodes
    // and write the nodes to a two-dimensional array
    // called FEM_Nodes


    if ( Number_of_EightNodeBrick_Nodes != 0)
    {

        eightnodebricknodesinputfile.open(eightnodebricknodesfilename , ios::in);


        if(! eightnodebricknodesinputfile)
        {
            fprintf(stderr, "\n Cannot open the eightnodebricknodesinputfile file \n" );
            return 1;
        }


        float dataPoint = 0;
        int nodenumber = 0;



        for (int i = 0; i < Number_of_EightNodeBrick_Nodes; i++)
        {

            eightnodebricknodesinputfile >> nodenumber;
            FEM_Nodes[i][1] = nodenumber;


            for (int j = 2; j < 5; j++)
            {
                eightnodebricknodesinputfile >> dataPoint;
                FEM_Nodes[i][j] = dataPoint;
            }
        }

        eightnodebricknodesinputfile.close();

    }



    // ************************
    // Check if there is any data for beam nodes
    // and continue write the nodes to a two-dimensional
    // array called FEM_Nodes


    if ( Number_of_Beam_Nodes != 0)
    {

        beamnodesinputfile.open(beamnodesfilename , ios::in);


        if(! beamnodesinputfile)
        {
            fprintf(stderr, "\n Cannot open the eightnodebricknodesinputfile file \n" );
            return 1;
        }


        float dataPoint = 0;
        int nodenumber = 0;


        for (int i = Number_of_EightNodeBrick_Nodes; i < total_number_of_nodes; i++)
        {

            beamnodesinputfile >> nodenumber;
            FEM_Nodes[i][1] = nodenumber;

            for (int j = 2; j < 5; j++)
            {
                beamnodesinputfile >> dataPoint;
                FEM_Nodes[i][j] = dataPoint;
            }
        }

        beamnodesinputfile.close();

    }



    // ***********************************************************************************
    // Check if there is any data for eightnodebrick elements
    // and count number of the elements


    int Number_of_EightNodeBrick_Elements = 0;
    int Number_of_Beam_Elements = 0;


    const char* eightnodebrickelementsfilename;
    const char* beamelementsfilename;

    ifstream eightnodebrickelementsinputfile;
    ifstream beamelementsinputfile;


    string eightnodebrickelementsfilenamestr;
    cout << "\n Enter the file name with soil elements: ";
    getline(cin, eightnodebrickelementsfilenamestr);


    string beamelementsfilenamestr;
    cout << "\n Enter the file name with beam elements: ";
    getline(cin, beamelementsfilenamestr);



    if (!eightnodebrickelementsfilenamestr.empty())
    {

        eightnodebrickelementsfilename = eightnodebrickelementsfilenamestr.c_str();

        eightnodebrickelementsinputfile.open(eightnodebrickelementsfilename , ios::in);


        if(! eightnodebrickelementsinputfile)
        {
            fprintf(stderr, "\n Cannot open the eightnodebrickelementsinputfile file \n" );
            return 1;
        }


        string camma;

        while (!eightnodebrickelementsinputfile.eof())
        {
            getline ( eightnodebrickelementsinputfile , camma);
            Number_of_EightNodeBrick_Elements++;
        }

        cout << "\n # of EightNodeBrick Elements is : " << Number_of_EightNodeBrick_Elements << endl;

        eightnodebrickelementsinputfile.close();

    }


    // *********************************************
    // Check if there is any data for beam elements
    // and count number of the elements


    if (!beamelementsfilenamestr.empty())
    {

        beamelementsfilename = beamelementsfilenamestr.c_str();

        beamelementsinputfile.open(beamelementsfilename , ios::in);


        if(! beamelementsinputfile)
        {
            fprintf(stderr, "\n Cannot open the beamelementsinputfile file \n" );
            return 1;
        }

        string camma;

        while (!beamelementsinputfile.eof())
        {
            getline ( beamelementsinputfile , camma);
            Number_of_Beam_Elements++;
        }

        cout << "\n # of Beam Elements is : " << Number_of_Beam_Elements << endl;

        beamelementsinputfile.close();

    }


    // ************************************

    int total_number_of_elements = Number_of_EightNodeBrick_Elements + Number_of_Beam_Elements;

    int celltypes[total_number_of_elements];
    int nconnectivity_8nodebrick = 8 * Number_of_EightNodeBrick_Elements;
    int nconnectivity_beam = 2 * Number_of_Beam_Elements;
    int connectivity_8nodebrick[nconnectivity_8nodebrick];
    int connectivity_beam[nconnectivity_beam];


    // array for storing the material number for each element
    int Element_Materials[total_number_of_elements];


    // **************************************************
    // Check if there is any data for soil elements
    // and write the elements into one-dimensional array
    // called connectivity_8nodebrick_temp which just knows about the
    // connectivities


    if ( Number_of_EightNodeBrick_Elements != 0)
    {

        for (int i = 0; i < Number_of_EightNodeBrick_Elements; i++)
        {
            celltypes[i] = 12;
        }


        eightnodebrickelementsinputfile.open(eightnodebrickelementsfilename , ios::in);


        if(! eightnodebrickelementsinputfile)
        {
            fprintf(stderr, "\n Cannot open the eightnodebrickelementsinputfile file \n" );
            return 1;
        }


        int dataPoint2 = 0;
        int cols2 = 8;

        int connectivity_8nodebrick_temp[nconnectivity_8nodebrick];


        for (int i = 0; i < Number_of_EightNodeBrick_Elements; i++)
        {

            eightnodebrickelementsinputfile >> dataPoint2;

            for (int j = 0; j < cols2; j++)
            {
                eightnodebrickelementsinputfile >> dataPoint2;
                int k = i * 8 + j;
                connectivity_8nodebrick_temp[k] = dataPoint2;
            }

            eightnodebrickelementsinputfile >> dataPoint2;
            Element_Materials[i] = dataPoint2;
        }

        eightnodebrickelementsinputfile.close();


        // **************************************************
        // connectivity nodes should start from zero and should be from
        // zero to (Number_of_EightNodeBrick_Elements-1)
        // Here connectivity_8nodebrick_temp components which compare to Node Numbers
        // which are from zero to (Number_of_EightNodeBrick_Elements-1) and write
        // them into new one-dimensional array called conn_soil

        for (int i = 0; i < nconnectivity_8nodebrick; i++)
        {
            for (int j = 0; j < total_number_of_nodes; j++)
            {
                if (connectivity_8nodebrick_temp[i] == FEM_Nodes[j][1])
                {
                    connectivity_8nodebrick[i] = FEM_Nodes[j][0];
                    break;
                }
            }
        }


    }



    // **************************************************
    // Check if there is any data for beam elements
    // and write the elements into one-dimensional array
    // called connectivity_beam_temp which just knows about the
    // connectivities

    if ( Number_of_Beam_Elements != 0)
    {

        for (int i = Number_of_EightNodeBrick_Elements; i < total_number_of_elements; i++)
        {
            celltypes[i] = 3;
        }

        beamelementsinputfile.open(beamelementsfilename , ios::in);


        if(! beamelementsinputfile)
        {
            fprintf(stderr, "\n Cannot open the beamelementsinputfile file \n" );
            return 1;
        }


        int connectivity_beam_temp[nconnectivity_beam];

        int dataPoint2 = 0;
        int cols2 = 2;

        for (int i = 0; i < Number_of_Beam_Elements; i++)
        {
            beamelementsinputfile >> dataPoint2;

            for (int j = 0; j < cols2; j++)
            {
                beamelementsinputfile >> dataPoint2;
                int k = i * 2 + j;
                connectivity_beam_temp[k] = dataPoint2;
            }

            beamelementsinputfile >> dataPoint2;
            Element_Materials[i + Number_of_EightNodeBrick_Elements] = dataPoint2;

        }

        beamelementsinputfile.close();


        // connectivity nodes should start from zero and should be from
        // zero to (Number_of_Beam_Elements-1)
        // Here connectivity_beam_temp components which compare to Node Numbers
        // which are from zero to (Number_of_Beam_Elements-1) and write
        // them into new one-dimensional array called connectivity_beam

        for (int i = 0; i < nconnectivity_beam; i++)
        {
            for (int j = 0; j < total_number_of_nodes; j++)
            {
                if (connectivity_beam_temp[i] == FEM_Nodes[j][1])
                {
                    connectivity_beam[i] = FEM_Nodes[j][0];
                    break;
                }
            }
        }

    }



    //**********************************************************************

    int total_ndofs = ndofs_soil + ndofs_beam;
    ifstream nodedisplacementsinputfile;


    // Name of the vector response to be saved which in this case
    // it is nodal displacements with 3-dofs
    const char* const varnames[] = {"Displacement"};


    int Number_of_Time_Steps = 0;

    const char* nodedisplacementsfilename;


    string nodedisplacementfilenamestr;
    cout << "\n Enter the file name with node displacements: ";
    getline(cin, nodedisplacementfilenamestr);


    // **************************************************
    // Check if there is any data for nodal displacements
    // and count the number of time steps.

    if (!nodedisplacementfilenamestr.empty())
    {

        nodedisplacementsfilename = nodedisplacementfilenamestr.c_str();

        nodedisplacementsinputfile.open(nodedisplacementsfilename , ios::in);


        if(! nodedisplacementsinputfile)
        {
            fprintf(stderr, "\n Cannot open the nodedisplacementsinputfile file \n" );
            return 1;
        }


        string camma;


        while (!nodedisplacementsinputfile.eof())
        {
            getline ( nodedisplacementsinputfile , camma);
            Number_of_Time_Steps++;
        }

        cout << "\n # of Time Steps is : " << Number_of_Time_Steps << endl;
        cout << endl;

        nodedisplacementsinputfile.close();

    }


    // **************************************************
    // Check if there is any data for nodal displacements
    // and write them into two-dimensional array
    // called vars which number of the rows is the number
    // of the time steps and number of columns is the total
    // number of dofs.

    if ( Number_of_Time_Steps != 0)
    {

        nodedisplacementsinputfile.open(nodedisplacementsfilename , ios::in);


        if(! nodedisplacementsinputfile)
        {
            fprintf(stderr, "\n Cannot open the nodedisplacementsinputfile file \n" );
            return 1;
        }


        float** vars = new float*[Number_of_Time_Steps];

        for(int i = 0; i < Number_of_Time_Steps; ++i)
        {
            vars[i] = new float[total_ndofs];
        }


        double dataPoint3 = 0;


        ///////////////////////////////////////////////////////////////////

        // There will be Number_of_Time_Steps of outputfiles

        for (int i = 0; i < Number_of_Time_Steps; i++)
        {

            nodedisplacementsinputfile >> dataPoint3;

            for (int j = 0; j < total_ndofs; j++)
            {
                nodedisplacementsinputfile >> dataPoint3;
                vars[i][j]  = dataPoint3;
            }


            ////////////////////////////////////////
            // converting integer to string
            string integertostring;
            stringstream out;
            out << i;
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


            outputfile << "CELLS " << total_number_of_elements << " " << Number_of_EightNodeBrick_Elements * 9 + Number_of_Beam_Elements * 3 << endl;


            for (int m = 0; m < Number_of_EightNodeBrick_Elements; m++)
            {
                outputfile << "8" << "  ";

                for (int n = 0; n < 8; n++)
                {
                    outputfile << connectivity_8nodebrick[m * 8 + n] << "  ";
                }

                outputfile << endl;
            }


            for (int m = 0; m < Number_of_Beam_Elements; m++)
            {
                outputfile << "2" << "  ";

                for (int n = 0; n < 2; n++)
                {
                    outputfile << connectivity_beam[m * 2 + n] << "  ";
                }

                outputfile << endl;
            }



            outputfile << "CELL_TYPES " << total_number_of_elements << endl;

            for (int x = 0; x < Number_of_EightNodeBrick_Elements; x++)
            {
                outputfile << celltypes[x] << endl;
            }


            for (int x = Number_of_EightNodeBrick_Elements; x < total_number_of_elements; x++)
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

            outputfile << "POINT_DATA " << total_number_of_nodes << endl;
            outputfile << "VECTORS " << *varnames << " float" << endl;


            for (int y = 0; y < total_number_of_nodes; y++)
            {

                for (int z = 0; z < 3; z++)
                {
                    outputfile << vars[i][y * 3 + z] << "  ";
                }

                outputfile << endl;
            }

        } // end of for loop on Number_of_Time_Steps

    } // end of if condition on Number_of_Time_Steps!=0

    ///////////////////////////////////////////////////////////////////


    return 0;
}



