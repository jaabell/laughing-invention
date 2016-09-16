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
//! - ElementNumber: unique element object tag
//! - node_numb_#: eight node coordinates
//! - MaterialNumber: material tag associated with previsouly-difined NDMaterialLT object
//! - AccelerationFieldNumber: acceleration field tag associated with previsouly-defined accelerationfield object





int add_element_brick_20node(int ElementNumber,
                             int node_numb_1,
                             int node_numb_2,
                             int node_numb_3,
                             int node_numb_4,
                             int node_numb_5,
                             int node_numb_6,
                             int node_numb_7,
                             int node_numb_8,
                             int node_numb_9,
                             int node_numb_10,
                             int node_numb_11,
                             int node_numb_12,
                             int node_numb_13,
                             int node_numb_14,
                             int node_numb_15,
                             int node_numb_16,
                             int node_numb_17,
                             int node_numb_18,
                             int node_numb_19,
                             int node_numb_20,
                             int MaterialNumber)
{
    NDMaterialLT* ndmaterialLT = 0;
    ndmaterialLT = theDomain.getNDMaterialLT(MaterialNumber);

    if (ndmaterialLT == NULL)
    {
        cerr << "Error: (add_element_brick_20node) memory allocation problem for ndmaterialLT!" << endl;
        return -1;
    }


    Element* theElement = 0;
    theElement = new TwentyNodeBrick(ElementNumber, node_numb_1, node_numb_2, node_numb_3, node_numb_4, node_numb_5, node_numb_6, node_numb_7, node_numb_8, node_numb_9, node_numb_10, node_numb_11, node_numb_12, node_numb_13, node_numb_14, node_numb_15, node_numb_16, node_numb_17, node_numb_18, node_numb_19, node_numb_20, ndmaterialLT);

    if (theElement == NULL)
    {
        cerr << "Error: (add_element_brick_20node) memory allocation problem for theElement!" << endl;
        return -1;
    }

    if (theDomain.addElement(theElement) == false)
    {
        cerr << "WARNING: (add_element_brick_20node) could not add element to the domain\n";
        cerr << "Element number: " << ElementNumber << endln;
        return -1;
    }

    if(ndmaterialLT->getClassTag()==ND_TAG_ElasticIsotropic3DLT)// [Sumeet September, 2016]
        theElement->setElasticMaterialStatus(0);

    return 0;

};





