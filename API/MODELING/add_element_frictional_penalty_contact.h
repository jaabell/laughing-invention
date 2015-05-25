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
// PROGRAMMER:        Mario Martinelli, Nima Tafazzoli, Boris Jeremic
//
// DATE:
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////



//! Inputs:
//! - ElementNumber: unique element object tag
//! - iNode , jNode: end nodes
//! - Knormal:
//! - frictionRatio: Friction coefficient
//! - x_local_1, x_local_2, x_local_3: Vector components in global coordinates defining local x-axis of the element




int add_element_frictional_penalty_contact(int ElementNumber,
        int iNode,
        int jNode,
        double Knormal,
        double Ktangent,
        double Cnormal,
        double Ctangent,
        double frictionRatio,
        double x_local_1,
        double x_local_2,
        double x_local_3)
{
    Element* theElement = 0;
    theElement = new FrictionalPenaltyContact(ElementNumber,
            iNode,
            jNode,
            Knormal,
            Ktangent,
            Cnormal,
            Ctangent,
            frictionRatio,
            x_local_1,
            x_local_2,
            x_local_3);

    if (theElement == NULL)
    {
        cerr << "Error: (add_element_frictional_penalty_contact) memory allocation problem for theElement!" << endl;
        return -1;
    }

    if (theDomain.addElement(theElement) == false)
    {
        cerr << "WARNING (add_element_frictional_penalty_contact) could not add element to the domain\n";
        cerr << "Element Number: " << ElementNumber << endln;
        return -1;
    }

    return 0;

};

