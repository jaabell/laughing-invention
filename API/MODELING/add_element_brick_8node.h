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
// DATE:              September 2009
// UPDATE HISTORY:    July 2011
//
//
///////////////////////////////////////////////////////////////////////////////



//! Inputs:
//! - elementTag: unique element object tag
//! - node_numb_#: eight node coordinates
//! - materialTag: material tag associated with previsouly-defined NDMaterial object
//! - AccelerationFieldNumber: acceleration field tag associated with previsouly-defined accelerationfield object



int add_element_brick_8node(int ElementNumber,
                            int node_numb_1,
                            int node_numb_2,
                            int node_numb_3,
                            int node_numb_4,
                            int node_numb_5,
                            int node_numb_6,
                            int node_numb_7,
                            int node_numb_8,
                            int MaterialNumber)
{
    NDMaterial* ndmaterial = 0;
    ndmaterial = theDomain.getNDMaterial(MaterialNumber);

    if (ndmaterial == NULL)
    {
        cerr << "Error: (add_element_brick_8node) memory allocation problem for ndmaterial!" << endl;
        return -1;
    }


    Element* theElement = 0;
    theElement = new EightNodeBrick(ElementNumber, node_numb_1, node_numb_2, node_numb_3, node_numb_4, node_numb_5, node_numb_6, node_numb_7, node_numb_8, ndmaterial);

    if (theElement == NULL)
    {
        cerr << "Error: (add_element_brick_8node) memory allocation problem for theElement!" << endl;
        return -1;
    }

    if (theDomain.addElement(theElement) == false)
    {
        cerr << "WARNING: (add_element_brick_8node) could not add element to the domain\n";
        cerr << "Element number: " << ElementNumber << endln;
        return -1;
    }

    return 0;

};











