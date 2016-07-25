///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         CompGeoMech
// DATE:               October 2009 - Aug 2015
// UPDATE HISTORY:     See git repository.
// QUALITY ASSURANCE:  Developers have worked really hard to develop
//                     an extensive verification of developed implementation
//                     and with that can claim quality and fitness for intended
//                     purpose (modeling and simulation of Real ESSI Problems)
//                     within confines of verification effort
//
// LEGACY/DEFUNCT COPYLEFT (C):
//                     Woody's viral GPL-like license (adapted by BJ):
//                     ``This    source  code is Copyrighted in
//                     worldwide for  an  indefinite  period,  and anybody
//                     caught  using it without our permission, will be
//                     mighty good friends of ourn, cause we don't give
//                     a  darn.  Hack it. Compile it. Debug it. Run it.
//                     Yodel  it.  Enjoy it. We wrote it, that's all we
//                     wanted to do.''
//
/////////////////////////////////////////////////////////////////////////////


//! Inputs:
//! - NodeNumber: integer tag identifying node
//! - number_of_DOFs: number of degrees of freedom for node
//! - coordinate_x: x coordinate of the node
//! - coordinate_y: y coordinate of the node
//! - coordinate_z: z coordinate of the node

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


    if ( theDomain.addNode(theaddNode) == false )
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



