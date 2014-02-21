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
// PURPOSE:
//
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:
// PROGRAMMER:        Nima Tafazzoli, Boris Jeremic
//
// DATE:              August 2009
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


int add_element_penalty_for_applying_generalized_displacement(int ElementNumber,
        int Exist_Node,
        double penalty_stiffness,
        int direction)
{

    Element* theElement = 0;
    theElement = new PenaltyElementApplyDisplacement(ElementNumber, Exist_Node, direction, penalty_stiffness);

    if (theElement == NULL)
    {
        cerr << "Error: (add_element_penalty_for_applying_generalized_displacement) memory allocation problem for theElement!" << endl;
        return -1;
    }

    if (theDomain.addElement(theElement) == false)
    {
        cerr << "WARNING: (add_element_penalty_for_applying_generalized_displacement) could not add element to the domain\n";
        cerr << "Element number: " << ElementNumber << endln;
        return -1;
    }

    return 0;

};





