///////////////////////////////////////////////////////////////////////////////
//   COPYLEFT (C): Woody's viral GPL-like license (by BJ):
//                 ``This    source  code is Copyrighted in
//                 U.S.,  for  an  indefinite  period,  and anybody
//                 caught  using it without our permission, will be
//                 mighty good friends of ourn, cause we don't give
//                 a  darn.  Hack it. Compile it. Debug it. Run it.
//                 Yodel  it.  Enjoy it. We wrote it, that's all we
//                 wanted to do.''
//
// COPYRIGHT (C):     :-))
// PROJECT:           Object Oriented Finite Element Program
// FILE:
// PURPOSE:           Mesh Generator for Block which inputs are the size of
//                    the model in X, Y, and Z directions and the Element
//                    size in X,Y, and Z directions.
//
// LANGUAGE:          C++
// TARGET OS:
// PROGRAMMER:        Nima Tafazzoli, Boris Jeremic
// DATE:              October 2010
//
///////////////////////////////////////////////////////////////////////////////


#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <sstream>
#include "Matrix.h"
#include <StandardStream.h>
#include "Vector.h"


using namespace std;
StandardStream sserr;
OPS_Stream& opserr = sserr;
OPS_Stream* opserrPtr;



int main(int argc, char** argv)
{

    ofstream outputfile;
    ofstream outputfile2;
    outputfile.open("nodes.txt" , ios::out);
    outputfile2.open("elements.txt" , ios::out);


    double dimension_x;
    double dimension_y;
    double dimension_z;
    double element_size_x;
    double element_size_y;
    double element_size_z;


    cout << "\n Enter the size of the model in 'X' direction: ";
    cin >> dimension_x;

    cout << "\n Enter the size of the model in 'Y' direction: ";
    cin >> dimension_y;

    cout << "\n Enter the size of the model in 'Z' direction: ";
    cin >> dimension_z;

    cout << "\n Enter the size of the elements in 'X' direction: ";
    cin >> element_size_x;

    cout << "\n Enter the size of the elements in 'Y' direction: ";
    cin >> element_size_y;

    cout << "\n Enter the size of the elements in 'Z' direction: ";
    cin >> element_size_z;


    int x_nodes = (dimension_x / element_size_x) + 1;
    int y_nodes = (dimension_y / element_size_y) + 1;
    int z_nodes = (dimension_z / element_size_z) + 1;

    int total_number_of_nodes = x_nodes * y_nodes * z_nodes;
    int total_number_of_elements =  (x_nodes - 1) * (y_nodes - 1) * (z_nodes - 1);

    int nodenumber = 0;

    //    outputfile << "# of Nodes is : " << total_number_of_nodes << endl;
    //    outputfile << "# of Elements is : " << total_number_of_elements << endl;



    Matrix Nodes(total_number_of_nodes, 5);


    cout << "\n Creating Nodes ...\n";

    for (int k = 0; k < z_nodes; k++)
    {
        for (int i = 0; i < x_nodes; i++)
        {
            for (int j = 0; j < y_nodes; j++)
            {
                nodenumber++;
                Nodes(nodenumber - 1, 0) = nodenumber;
                Nodes(nodenumber - 1, 1) = i * element_size_x;
                Nodes(nodenumber - 1, 2) = j * element_size_y;
                Nodes(nodenumber - 1, 3) = k * element_size_z;

                outputfile << "add_node(" << nodenumber << ", 3, " << i* element_size_x << ", " << j* element_size_y << ", " << k* element_size_z << ");" << endl;

                //        outputfile << nodenumber << " " << i*element_size_x << " " << j*element_size_y << " " << k*element_size_z << endl;

            }
        }
    }


    outputfile.close();



    cout << " Creating Elements ...\n";


    int Node1 = 0;
    int Node2 = 0;
    int Node3 = 0;
    int Node4 = 0;
    int Node5 = 0;
    int Node6 = 0;
    int Node7 = 0;
    int Node8 = 0;

    int elementnumber = 0;



    for (int i = 0; i < total_number_of_nodes; i++)
    {

        bool Node1_exist = false;
        bool Node2_exist = false;
        bool Node3_exist = false;
        bool Node4_exist = false;
        bool Node5_exist = false;
        bool Node6_exist = false;
        bool Node7_exist = false;
        bool Node8_exist = false;


        for (int j = 0; j < total_number_of_nodes; j++)
        {

            if ((Nodes(j, 1) == Nodes(i, 1) + element_size_x) && (Nodes(j, 2) == Nodes(i, 2) + element_size_y) && (Nodes(j, 3) == Nodes(i, 3) + element_size_z))
            {
                Node1 = Nodes(j, 0);
                Node1_exist = true;
            }

            if ((Nodes(j, 1) == Nodes(i, 1)) && (Nodes(j, 2) == Nodes(i, 2) + element_size_y) && (Nodes(j, 3) == Nodes(i, 3) + element_size_z))
            {
                Node2 = Nodes(j, 0);
                Node2_exist = true;
            }

            if ((Nodes(j, 1) == Nodes(i, 1)) && (Nodes(j, 2) == Nodes(i, 2)) && (Nodes(j, 3) == Nodes(i, 3) + element_size_z))
            {
                Node3 = Nodes(j, 0);
                Node3_exist = true;
            }

            if ((Nodes(j, 1) == Nodes(i, 1) + element_size_x) && (Nodes(j, 2) == Nodes(i, 2)) && (Nodes(j, 3) == Nodes(i, 3) + element_size_z))
            {
                Node4 = Nodes(j, 0);
                Node4_exist = true;
            }

            if ((Nodes(j, 1) == Nodes(i, 1) + element_size_x) && (Nodes(j, 2) == Nodes(i, 2) + element_size_y) && (Nodes(j, 3) == Nodes(i, 3)))
            {
                Node5 = Nodes(j, 0);
                Node5_exist = true;
            }

            if ((Nodes(j, 1) == Nodes(i, 1)) && (Nodes(j, 2) == Nodes(i, 2) + element_size_y) && (Nodes(j, 3) == Nodes(i, 3)))
            {
                Node6 = Nodes(j, 0);
                Node6_exist = true;
            }

            if ((Nodes(j, 1) == Nodes(i, 1)) && (Nodes(j, 2) == Nodes(i, 2)) && (Nodes(j, 3) == Nodes(i, 3)))
            {
                Node7 = Nodes(j, 0);
                Node7_exist = true;
            }

            if ((Nodes(j, 1) == Nodes(i, 1) + element_size_x) && (Nodes(j, 2) == Nodes(i, 2)) && (Nodes(j, 3) == Nodes(i, 3)))
            {
                Node8 = Nodes(j, 0);
                Node8_exist = true;
            }

        }


        if ( (Node1_exist == true) && (Node2_exist == true) && (Node3_exist == true) && (Node4_exist == true)
                && (Node5_exist == true) && (Node6_exist == true) && (Node7_exist == true) && (Node8_exist == true) )
        {
            elementnumber++;

            outputfile2 << "add_element_brick_8node_elastic(" << elementnumber << ", " << Node1 << ", " << Node2 << ", " << Node3 << ", "
                        << Node4 << ", " << Node5 << ", " << Node6 << ", " << Node7 << ", " << Node8 << ", 1, 1);" << endl;

            //       outputfile2 << elementnumber << " " << Node1 << " " << Node2 << " " << Node3 << " "
            //       << Node4 << " " << Node5 << " " << Node6 << " " << Node7 << " " << Node8 << endl;
        }


    }

    outputfile2.close();

    cout << " Mesh Generation is Done!\n";

    exit(0);
}
