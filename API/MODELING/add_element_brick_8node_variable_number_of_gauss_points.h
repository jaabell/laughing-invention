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


int add_element_brick_8node_variable_number_of_gauss_points(int ElementNumber,
        int number_of_gauss_points,
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
    NDMaterialLT* ndmaterialLT = 0;
    ndmaterialLT = theDomain.getNDMaterialLT(MaterialNumber);

    if (ndmaterialLT == NULL)
    {
        cerr << "Error: (add_element_brick_8node_variable_number_of_gauss_points) memory allocation problem for ndmaterialLT!" << endl;
        return -1;
    }


    Element* theElement = 0;

    switch(number_of_gauss_points){

        case 1: theElement = new EightNodeBrickOrderOne(ElementNumber,node_numb_1, node_numb_2, node_numb_3, node_numb_4, node_numb_5, node_numb_6, node_numb_7, node_numb_8, ndmaterialLT);
                break;
        case 2: theElement = new EightNodeBrickOrderTwo(ElementNumber,node_numb_1, node_numb_2, node_numb_3, node_numb_4, node_numb_5, node_numb_6, node_numb_7, node_numb_8, ndmaterialLT);
                break;
        case 3: theElement = new EightNodeBrickOrderThree(ElementNumber,node_numb_1, node_numb_2, node_numb_3, node_numb_4, node_numb_5, node_numb_6, node_numb_7, node_numb_8, ndmaterialLT);
                break;
        case 4: theElement = new EightNodeBrickOrderFour(ElementNumber,node_numb_1, node_numb_2, node_numb_3, node_numb_4, node_numb_5, node_numb_6, node_numb_7, node_numb_8, ndmaterialLT);
                break;
        case 5: theElement = new EightNodeBrickOrderFive(ElementNumber,node_numb_1, node_numb_2, node_numb_3, node_numb_4, node_numb_5, node_numb_6, node_numb_7, node_numb_8, ndmaterialLT);
                break;
        case 6: theElement = new EightNodeBrickOrderSix(ElementNumber,node_numb_1, node_numb_2, node_numb_3, node_numb_4, node_numb_5, node_numb_6, node_numb_7, node_numb_8, ndmaterialLT);
                break;
        default:  cerr << "Error: (add_element_brick_8node_variable_number_of_gauss_points) number_of_gauss_points should be between 1-6!";
    }


    if (theElement == NULL)
    {
        cerr << "Error: (add_element_brick_8node_variable_number_of_gauss_points) memory allocation problem for theElement!" << endl;
        return -1;
    }

    if (theDomain.addElement(theElement) == false)
    {
        cerr << "WARNING: (add_element_brick_8node_variable_number_of_gauss_points) could not add element to the domain\n";
        cerr << "Element number: " << ElementNumber << endln;
        return -1;
    }

    if(ndmaterialLT->getClassTag()==ND_TAG_ElasticIsotropic3DLT)// [Sumeet September, 2016]
        theElement->setElasticMaterialStatus(0);


    return 0;

};











