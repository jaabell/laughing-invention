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


int add_element_brick_variable_node_8_to_27(int ElementNumber, int number_of_gauss_points,
        int node_numb_1,  int node_numb_2,  int node_numb_3,
        int node_numb_4,  int node_numb_5,  int node_numb_6,
        int node_numb_7,  int node_numb_8,  int node_numb_9,
        int node_numb_10, int node_numb_11, int node_numb_12,
        int node_numb_13, int node_numb_14, int node_numb_15,
        int node_numb_16, int node_numb_17, int node_numb_18,
        int node_numb_19, int node_numb_20, int node_numb_21,
        int node_numb_22, int node_numb_23, int node_numb_24,
        int node_numb_25, int node_numb_26, int node_numb_27,
        int MaterialNumber)
{
    NDMaterial* ndmaterial = 0;
    ndmaterial = theDomain.getNDMaterial(MaterialNumber);

    if (ndmaterial == NULL)
    {
        cerr << "Error: (add_element_brick_variable_node) memory allocation problem for ndmaterial!" << endl;
        return -1;
    }


    Element* theElement = 0;
    theElement = new VariableNodeBrick(ElementNumber, number_of_gauss_points,
                                       node_numb_1, node_numb_2, node_numb_3, node_numb_4,
                                       node_numb_5, node_numb_6, node_numb_7, node_numb_8, node_numb_9,
                                       node_numb_10, node_numb_11, node_numb_12, node_numb_13, node_numb_14,
                                       node_numb_15, node_numb_16, node_numb_17, node_numb_18, node_numb_19,
                                       node_numb_20, node_numb_21, node_numb_22, node_numb_23, node_numb_24,
                                       node_numb_25, node_numb_26, node_numb_27,
                                       ndmaterial);

    if (theElement == NULL)
    {
        cerr << "Error: (add_element_brick_variable_node) memory allocation problem for theElement!" << endl;
        return -1;
    }

    if (theDomain.addElement(theElement) == false)
    {
        cerr << "WARNING: (add_element_brick_variable_node) could not add element to the domain\n";
        cerr << "Element number: " << ElementNumber << endln;
        return -1;
    }

    return 0;

};





