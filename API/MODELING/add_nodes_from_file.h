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
//! <h1>DSL of Constructing Nodes from File</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (September 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of constructing nodes from File.
//! \n
//! \n
//! For information about the function and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//!
//!


//! Inputs:
//! - inputnodesfile: name of the input file which has the nodal information

//! Format of input file:
//! It is the same as the inputs for creating the nodes. First argument should be
//! the node number following by number of dofs, x_coordinate, y_coordinate, z_coordinate

void add_nodes_from_file(const char *, int);
void add_nodes_from_file(std::string inputnodesfile, int ndof)
{

    int NodeTag;
    double x_coordinate;
    double y_coordinate;
    double z_coordinate;

    const char *nodesfromfile = inputnodesfile.c_str();

    ifstream inputnodes;

    if (! inputnodes)
    {
        cerr << "Error: (add_nodes_from_file) Can not open " << inputnodesfile << "!" << endl;
        abort();
    }
    else
    {
        cout << "       Adding " << ndof << "-DOF nodes from file " << inputnodesfile << " \n";
    }

    inputnodes.open(nodesfromfile , ios::in);

    int count = 0s;
    while (inputnodes >> NodeTag >> x_coordinate >> y_coordinate >> z_coordinate)
    {
        add_node(NodeTag, ndof, x_coordinate, y_coordinate, z_coordinate);
        count += 1;
    }
    cout << "       Finished! Added " << count << "nodes. \n";

    inputnodes.close();
};















