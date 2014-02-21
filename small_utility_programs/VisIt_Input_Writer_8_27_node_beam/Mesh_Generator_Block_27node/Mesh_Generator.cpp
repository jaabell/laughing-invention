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

using namespace std;


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


    int x_nodes = (dimension_x / element_size_x) * 2 + 1;
    int y_nodes = (dimension_y / element_size_y) * 2 + 1;
    int z_nodes = (dimension_z / element_size_z) * 2 + 1;

    int total_number_of_nodes = x_nodes * y_nodes * z_nodes;
    //     int total_number_of_elements =  (x_nodes-1) * (y_nodes-1) * (z_nodes-1) / 8;

    int nodenumber = 0;


    //    cout << "\n Number of Nodes is : " << total_number_of_nodes << endl;
    //    outputfile << "# of Elements is : " << total_number_of_elements << endl;


    double Nodes[total_number_of_nodes][5];


    cout << "\n Creating Nodes ...\n";

    for (int k = 0; k < z_nodes; k++)
    {
        for (int i = 0; i < x_nodes; i++)
        {
            for (int j = 0; j < y_nodes; j++)
            {
                nodenumber++;
                Nodes[nodenumber - 1][0] = nodenumber;
                Nodes[nodenumber - 1][1] = i * element_size_x * 0.5;
                Nodes[nodenumber - 1][2] = j * element_size_y * 0.5;
                Nodes[nodenumber - 1][3] = k * element_size_z * 0.5;

                if ((i % 2 == 0) && (j % 2 == 0) && (k % 2 == 0))
                {
                    Nodes[nodenumber - 1][4] = 1;
                }
                else
                {
                    Nodes[nodenumber - 1][4] = 0;
                }

                outputfile << Nodes[nodenumber - 1][0] << " " << Nodes[nodenumber - 1][1] << " "
                           << Nodes[nodenumber - 1][2] << " " << Nodes[nodenumber - 1][3] << " " << endl;
            }
        }
    }


    outputfile.close();



    cout << " Creating Elements ...\n";

    //     double Elements[total_number_of_elements][28];

    int Node1  = 0;
    int Node2  = 0;
    int Node3  = 0;
    int Node4  = 0;
    int Node5  = 0;
    int Node6  = 0;
    int Node7  = 0;
    int Node8  = 0;
    int Node9  = 0;
    int Node10 = 0;
    int Node11 = 0;
    int Node12 = 0;
    int Node13 = 0;
    int Node14 = 0;
    int Node15 = 0;
    int Node16 = 0;
    int Node17 = 0;
    int Node18 = 0;
    int Node19 = 0;
    int Node20 = 0;
    int Node21 = 0;
    int Node22 = 0;
    int Node23 = 0;
    int Node24 = 0;
    int Node25 = 0;
    int Node26 = 0;
    int Node27 = 0;

    int elementnumber = 0;



    for (int i = 0; i < total_number_of_nodes; i++)
    {

        bool Node1_exist  = false;
        bool Node2_exist  = false;
        bool Node3_exist  = false;
        bool Node4_exist  = false;
        bool Node5_exist  = false;
        bool Node6_exist  = false;
        bool Node7_exist  = false;
        bool Node8_exist  = false;
        bool Node9_exist  = false;
        bool Node10_exist = false;
        bool Node11_exist = false;
        bool Node12_exist = false;
        bool Node13_exist = false;
        bool Node14_exist = false;
        bool Node15_exist = false;
        bool Node16_exist = false;
        bool Node17_exist = false;
        bool Node18_exist = false;
        bool Node19_exist = false;
        bool Node20_exist = false;
        bool Node21_exist = false;
        bool Node22_exist = false;
        bool Node23_exist = false;
        bool Node24_exist = false;
        bool Node25_exist = false;
        bool Node26_exist = false;
        bool Node27_exist = false;

        if (Nodes[i][4] == 1)
        {

            for (int j = 0; j < total_number_of_nodes; j++)
            {

                if ((Nodes[j][1] == Nodes[i][1] + element_size_x) && (Nodes[j][2] == Nodes[i][2] + element_size_y) && (Nodes[j][3] == Nodes[i][3] + element_size_z))
                {
                    Node1 = Nodes[j][0];
                    Node1_exist = true;
                }

                if ((Nodes[j][1] == Nodes[i][1]) && (Nodes[j][2] == Nodes[i][2] + element_size_y) && (Nodes[j][3] == Nodes[i][3] + element_size_z))
                {
                    Node2 = Nodes[j][0];
                    Node2_exist = true;
                }

                if ((Nodes[j][1] == Nodes[i][1]) && (Nodes[j][2] == Nodes[i][2]) && (Nodes[j][3] == Nodes[i][3] + element_size_z))
                {
                    Node3 = Nodes[j][0];
                    Node3_exist = true;
                }

                if ((Nodes[j][1] == Nodes[i][1] + element_size_x) && (Nodes[j][2] == Nodes[i][2]) && (Nodes[j][3] == Nodes[i][3] + element_size_z))
                {
                    Node4 = Nodes[j][0];
                    Node4_exist = true;
                }

                if ((Nodes[j][1] == Nodes[i][1] + element_size_x) && (Nodes[j][2] == Nodes[i][2] + element_size_y) && (Nodes[j][3] == Nodes[i][3]))
                {
                    Node5 = Nodes[j][0];
                    Node5_exist = true;
                }

                if ((Nodes[j][1] == Nodes[i][1]) && (Nodes[j][2] == Nodes[i][2] + element_size_y) && (Nodes[j][3] == Nodes[i][3]))
                {
                    Node6 = Nodes[j][0];
                    Node6_exist = true;
                }

                if ((Nodes[j][1] == Nodes[i][1]) && (Nodes[j][2] == Nodes[i][2]) && (Nodes[j][3] == Nodes[i][3]))
                {
                    Node7 = Nodes[j][0];
                    Node7_exist = true;
                }

                if ((Nodes[j][1] == Nodes[i][1] + element_size_x) && (Nodes[j][2] == Nodes[i][2]) && (Nodes[j][3] == Nodes[i][3]))
                {
                    Node8 = Nodes[j][0];
                    Node8_exist = true;
                }

                if ((Nodes[j][1] == Nodes[i][1] + 0.5 * element_size_x) && (Nodes[j][2] == Nodes[i][2] + element_size_y) && (Nodes[j][3] == Nodes[i][3] + element_size_z))
                {
                    Node9 = Nodes[j][0];
                    Node9_exist = true;
                }

                if ((Nodes[j][1] == Nodes[i][1]) && (Nodes[j][2] == Nodes[i][2] + 0.5 * element_size_y) && (Nodes[j][3] == Nodes[i][3] + element_size_z))
                {
                    Node10 = Nodes[j][0];
                    Node10_exist = true;
                }

                if ((Nodes[j][1] == Nodes[i][1] + 0.5 * element_size_x) && (Nodes[j][2] == Nodes[i][2]) && (Nodes[j][3] == Nodes[i][3] + element_size_z))
                {
                    Node11 = Nodes[j][0];
                    Node11_exist = true;
                }

                if ((Nodes[j][1] == Nodes[i][1] + element_size_x) && (Nodes[j][2] == Nodes[i][2] + 0.5 * element_size_y) && (Nodes[j][3] == Nodes[i][3] + element_size_z))
                {
                    Node12 = Nodes[j][0];
                    Node12_exist = true;
                }

                if ((Nodes[j][1] == Nodes[i][1] + 0.5 * element_size_x) && (Nodes[j][2] == Nodes[i][2] + element_size_y) && (Nodes[j][3] == Nodes[i][3]))
                {
                    Node13 = Nodes[j][0];
                    Node13_exist = true;
                }

                if ((Nodes[j][1] == Nodes[i][1]) && (Nodes[j][2] == Nodes[i][2] + 0.5 * element_size_y) && (Nodes[j][3] == Nodes[i][3]))
                {
                    Node14 = Nodes[j][0];
                    Node14_exist = true;
                }

                if ((Nodes[j][1] == Nodes[i][1] + 0.5 * element_size_x) && (Nodes[j][2] == Nodes[i][2]) && (Nodes[j][3] == Nodes[i][3]))
                {
                    Node15 = Nodes[j][0];
                    Node15_exist = true;
                }

                if ((Nodes[j][1] == Nodes[i][1] + element_size_x) && (Nodes[j][2] == Nodes[i][2] + 0.5 * element_size_y) && (Nodes[j][3] == Nodes[i][3]))
                {
                    Node16 = Nodes[j][0];
                    Node16_exist = true;
                }

                if ((Nodes[j][1] == Nodes[i][1] + element_size_x) && (Nodes[j][2] == Nodes[i][2] + element_size_y) && (Nodes[j][3] == Nodes[i][3] + 0.5 * element_size_z))
                {
                    Node17 = Nodes[j][0];
                    Node17_exist = true;
                }

                if ((Nodes[j][1] == Nodes[i][1]) && (Nodes[j][2] == Nodes[i][2] + element_size_y) && (Nodes[j][3] == Nodes[i][3] + 0.5 * element_size_z))
                {
                    Node18 = Nodes[j][0];
                    Node18_exist = true;
                }

                if ((Nodes[j][1] == Nodes[i][1]) && (Nodes[j][2] == Nodes[i][2]) && (Nodes[j][3] == Nodes[i][3] + 0.5 * element_size_z))
                {
                    Node19 = Nodes[j][0];
                    Node19_exist = true;
                }

                if ((Nodes[j][1] == Nodes[i][1] + element_size_x) && (Nodes[j][2] == Nodes[i][2]) && (Nodes[j][3] == Nodes[i][3] + 0.5 * element_size_z))
                {
                    Node20 = Nodes[j][0];
                    Node20_exist = true;
                }

                if ((Nodes[j][1] == Nodes[i][1] + 0.5 * element_size_x) && (Nodes[j][2] == Nodes[i][2] + 0.5 * element_size_y) && (Nodes[j][3] == Nodes[i][3] + 0.5 * element_size_z))
                {
                    Node21 = Nodes[j][0];
                    Node21_exist = true;
                }

                if ((Nodes[j][1] == Nodes[i][1] + 0.5 * element_size_x) && (Nodes[j][2] == Nodes[i][2] + element_size_y) && (Nodes[j][3] == Nodes[i][3] + 0.5 * element_size_z))
                {
                    Node22 = Nodes[j][0];
                    Node22_exist = true;
                }

                if ((Nodes[j][1] == Nodes[i][1]) && (Nodes[j][2] == Nodes[i][2] + 0.5 * element_size_y) && (Nodes[j][3] == Nodes[i][3] + 0.5 * element_size_z))
                {
                    Node23 = Nodes[j][0];
                    Node23_exist = true;
                }

                if ((Nodes[j][1] == Nodes[i][1] + 0.5 * element_size_x) && (Nodes[j][2] == Nodes[i][2]) && (Nodes[j][3] == Nodes[i][3] + 0.5 * element_size_z))
                {
                    Node24 = Nodes[j][0];
                    Node24_exist = true;
                }

                if ((Nodes[j][1] == Nodes[i][1] + element_size_x) && (Nodes[j][2] == Nodes[i][2] + 0.5 * element_size_y) && (Nodes[j][3] == Nodes[i][3] + 0.5 * element_size_z))
                {
                    Node25 = Nodes[j][0];
                    Node25_exist = true;
                }

                if ((Nodes[j][1] == Nodes[i][1] + 0.5 * element_size_x) && (Nodes[j][2] == Nodes[i][2] + 0.5 * element_size_y) && (Nodes[j][3] == Nodes[i][3] + element_size_z))
                {
                    Node26 = Nodes[j][0];
                    Node26_exist = true;
                }

                if ((Nodes[j][1] == Nodes[i][1] + 0.5 * element_size_x) && (Nodes[j][2] == Nodes[i][2] + 0.5 * element_size_y) && (Nodes[j][3] == Nodes[i][3]))
                {
                    Node27 = Nodes[j][0];
                    Node27_exist = true;
                }

            }

            if ( (Node1_exist == true) && (Node2_exist == true) && (Node3_exist == true) && (Node4_exist == true)
                    && (Node5_exist == true) && (Node6_exist == true) && (Node7_exist == true) && (Node8_exist == true)
                    && (Node9_exist == true) && (Node10_exist == true) && (Node11_exist == true) && (Node12_exist == true)
                    && (Node13_exist == true) && (Node14_exist == true) && (Node15_exist == true) && (Node16_exist == true)
                    && (Node17_exist == true) && (Node18_exist == true) && (Node19_exist == true) && (Node20_exist == true)
                    && (Node21_exist == true) && (Node22_exist == true) && (Node23_exist == true) && (Node24_exist == true)
                    && (Node25_exist == true) && (Node26_exist == true) && (Node27_exist == true) )
            {
                elementnumber++;
                outputfile2 << elementnumber << " " << Node1 << " " << Node2 << " " << Node3 << " "
                            << Node4 << " " << Node5 << " " << Node6 << " " << Node7 << " " << Node8 << " "
                            << Node9 << " " << Node10 << " " << Node11 << " " << Node12 << " " << Node13 << " "
                            << Node14 << " " << Node15 << " " << Node16 << " " << Node17 << " " << Node18 << " "
                            << Node19 << " " << Node20 << " " << Node21 << " " << Node22 << " " << Node23 << " "
                            << Node24 << " " << Node25 << " " << Node26 << " " << Node27 << endl;
            }

        }

    }



    outputfile2.close();

    cout << " Mesh Generation is Done!\n";

    exit(0);
}
