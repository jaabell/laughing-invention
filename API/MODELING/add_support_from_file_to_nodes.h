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
//! <h1>DSL of Adding Support to Nodes from File</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (December 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of adding support to nodes from file.
//! \n
//! \n
//! For information about the function and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//!
//!


//! Inputs:
//! - inputnodesfixity: name of the input file which has the fixities of nodes

//! Format of input file:
//! - First arguments should be Fix (for fixing just one specified dof) or Fixall (for fixing all dofs of a
//! specified node).
//! In case of "Fix" it should be followed by the node number and dof to be fixed and in case of "Fixall",
//! it should be followed by the node number.


int add_support_from_file_to_nodes(const char *);
int add_support_from_file_to_nodes(const char *inputnodesfixity)
{

    const char *nodesfixityfromfile = inputnodesfixity;

    ifstream inputfixity;

    if (! inputfixity)
    {
        cerr << "Error: (add_support_from_file_to_nodes) Can not open " << inputnodesfixity << "!" << endl;
        return -1;
    }

    inputfixity.open(nodesfixityfromfile , ios::in);

    string typeoffixity;

    while (!inputfixity.eof())
    {

        inputfixity >> typeoffixity;


        if (typeoffixity.compare("Fix") == 0)
        {
            int NodeNumber;
            int dof_number;

            inputfixity >> NodeNumber >> dof_number;

            add_support_to_node(NodeNumber, dof_number);
        }

        else if (typeoffixity.compare("Fixall") == 0)
        {
            int NodeNumber;

            inputfixity >> NodeNumber;

            add_support_to_all_dofs_of_node(NodeNumber);

        }


    }

    inputfixity.close();

    return 0;

};






