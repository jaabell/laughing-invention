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
// PROGRAMMER:        Jose Abell, Boris Jeremic
//
// DATE:              June 2013

//
///////////////////////////////////////////////////////////////////////////////


#include "siunit.h"
#include "quantity.h"
#include "unitdefinitions.h"


Quantity get_nodal_coordinate( int node, int which_coord )
{

    Node* theNode = theDomain.getNode(node);
    const Vector& coords = theNode->getCrds();

    Quantity coord(coords[which_coord - 1], meters);
    return coord;
};
