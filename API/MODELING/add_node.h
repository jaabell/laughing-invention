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
// DATE:              May 2009
// UPDATE HISTORY:    August 2009
//
///////////////////////////////////////////////////////////////////////////////


//!
//! @mainpage
//! \n
//! <div class="contents">
//! <h1>DSL of Constructing a Node</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (September 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of constructing a node.
//! \n
//! \n
//! For information about the function and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//!
//!


//! Inputs:
//! - NodeNumber: integer tag identifying node
//! - number_of_DOFs: number of degrees of freedom for node
//! - coordinate_x: x coordinate of the node
//! - coordinate_y: y coordinate of the node
//! - coordinate_z: z coordinate of the node



//! \n
//! \n
//! \n
//! <B> For information about the theory behind this function and its arguments, please consult the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/CompGeomechanicsLectureNotes/CompGeomechanicsLectureNotes.html" target="_blank">lecture notes. </A></div>
//! \n
//! \n


int add_node(int NodeNumber,
             int number_of_DOFs,
             double coordinate_x,
             double coordinate_y,
             double coordinate_z)
{
    Node* theaddNode = 0;
    theaddNode = new Node(NodeNumber, number_of_DOFs, coordinate_x, coordinate_y, coordinate_z );

    if (theaddNode == NULL)
    {
        cerr << "Error: (add_node) memory for adding node can not be allocated!" << endl;
        return -1;
    }


    if( theDomain.addNode(theaddNode) == false )
    {
        cerr << "Error: (add_node) Node " << NodeNumber << " could not be added to the domain " << endl;
        return -1;
    }

    //     theaddNode = 0;
    //     theaddNode = NULL;
    //     free(theaddNode);
    //     delete theaddNode;

    return 0;

};



