///////////////////////////////////////////////////////////////////////////////
//   COPYLEFT (C): Woody's viral LGPL (by BJ):
//                 ``This    source  code is Copyrighted in
//                 U.S.,  for  an  indefinite  period,  and anybody
//                 caught  using it without our permission, will be
//                 mighty good friends of ourn, cause we don't give
//                 a  darn.  Hack it. Compile it. Debug it. Run it.
//                 Yodel  it.  Enjoy it. We wrote it, that's all we
//                 wanted to do.''
//
//
// COPYRIGHT (C):     LGPL
// PROJECT:           Object Oriented Finite Element Program
//
// PURPOSE:           Domain Specific Language (DSL)
//
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:
// PROGRAMMER:        Nima Tafazzoli, Boris Jeremic
//
// DATE:              January 2010
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


//!
//! @mainpage
//! \n
//! <div class="contents">
//! <h1>DSL of Adding Equal DOF to Nodes from File</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (December 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of adding equal dof to nodes from file.
//! \n
//! \n
//! For information about the function and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//!
//!


//! Inputs:
//! - inputequaldofnodes: name of the input file which equaldof information of nodes to be connected

//! Format of input file:
//! - The first argument should be the number of dofs for each node which are going to be connected following by
//! retained and constrained node numbers, and dof numbers to be connected.

void add_equaldof_to_two_nodes_from_file(const char*);
void add_equaldof_to_two_nodes_from_file(const char* inputequaldofnodes)
{

    const char* nodesequaldoffromfile = inputequaldofnodes;

    ifstream inputequaldof;

    if(! inputequaldof)
    {
        cerr << "Error: (add_equaldof_to_two_nodes_from_file) Can not open " << inputequaldofnodes << "!" << endl;
        abort();
    }

    inputequaldof.open(nodesequaldoffromfile , ios::in);

    int numberofconnections;

    while (!inputequaldof.eof())
    {

        inputequaldof >> numberofconnections;


        if (numberofconnections == 1)
        {
            int nodeRetain;
            int nodeConstr;
            int dofID1;

            inputequaldof >> nodeRetain >> nodeConstr >> dofID1;

            add_equaldof_to_two_nodes(nodeRetain, nodeConstr, dofID1);
        }

        else if (numberofconnections == 2)
        {
            int nodeRetain;
            int nodeConstr;
            int dofID1;
            int dofID2;

            inputequaldof >> nodeRetain >> nodeConstr >> dofID1 >> dofID2;

            add_equaldof_to_two_nodes(nodeRetain, nodeConstr, dofID1, dofID2);
        }

        else if (numberofconnections == 3)
        {
            int nodeRetain;
            int nodeConstr;
            int dofID1;
            int dofID2;
            int dofID3;

            inputequaldof >> nodeRetain >> nodeConstr >> dofID1 >> dofID2 >> dofID3;

            add_equaldof_to_two_nodes(nodeRetain, nodeConstr, dofID1, dofID2, dofID3);
        }

        else if (numberofconnections == 4)
        {
            int nodeRetain;
            int nodeConstr;
            int dofID1;
            int dofID2;
            int dofID3;
            int dofID4;

            inputequaldof >> nodeRetain >> nodeConstr >> dofID1 >> dofID2 >> dofID3 >> dofID4;

            add_equaldof_to_two_nodes(nodeRetain, nodeConstr, dofID1, dofID2, dofID3, dofID4);
        }

        else if (numberofconnections == 5)
        {
            int nodeRetain;
            int nodeConstr;
            int dofID1;
            int dofID2;
            int dofID3;
            int dofID4;
            int dofID5;

            inputequaldof >> nodeRetain >> nodeConstr >> dofID1 >> dofID2 >> dofID3 >> dofID4 >> dofID5;

            add_equaldof_to_two_nodes(nodeRetain, nodeConstr, dofID1, dofID2, dofID3, dofID4, dofID5);
        }

        else if (numberofconnections == 6)
        {
            int nodeRetain;
            int nodeConstr;
            int dofID1;
            int dofID2;
            int dofID3;
            int dofID4;
            int dofID5;
            int dofID6;

            inputequaldof >> nodeRetain >> nodeConstr >> dofID1 >> dofID2 >> dofID3 >> dofID4 >> dofID5 >> dofID6;

            add_equaldof_to_two_nodes(nodeRetain, nodeConstr, dofID1, dofID2, dofID3, dofID4, dofID5, dofID6);
        }

        else if (numberofconnections == 7)
        {
            int nodeRetain;
            int nodeConstr;
            int dofID1;
            int dofID2;
            int dofID3;
            int dofID4;
            int dofID5;
            int dofID6;
            int dofID7;

            inputequaldof >> nodeRetain >> nodeConstr >> dofID1 >> dofID2 >> dofID3 >> dofID4 >> dofID5 >> dofID6 >> dofID7;

            add_equaldof_to_two_nodes(nodeRetain, nodeConstr, dofID1, dofID2, dofID3, dofID4, dofID5, dofID6, dofID7);
        }

    }

    inputequaldof.close();

};















