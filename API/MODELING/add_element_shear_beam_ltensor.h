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
// PROGRAMMER:        Jose Abell, Nima Tafazzoli
//
// DATE:              September 2013
// UPDATE HISTORY:
//
//
///////////////////////////////////////////////////////////////////////////////

int add_element_shear_beam_ltensor(int ElementNumber,
                                   int node_numb_1,
                                   int node_numb_2,
                                   double area,
                                   int MaterialNumber)
{
    NDMaterialLT *ndmaterial = 0;
    ndmaterial = theDomain.getNDMaterialLT(MaterialNumber);

    if (ndmaterial == NULL)
    {
        cerr << "Error: (add_element_shear_beam_ltensor) memory allocation problem for ndmaterial!" << endl;
        return -1;
    }

    Element *theElement = 0;
    theElement = new ShearBeamLT(ElementNumber, node_numb_1, node_numb_2, area, ndmaterial);

    if (theElement == NULL)
    {
        cerr << "Error: (add_element_shear_beam_ltensor) memory allocation problem for theElement!" << endl;
        return -1;
    }

    if (theDomain.addElement(theElement) == false)
    {
        cerr << "WARNING: (add_element_shear_beam_ltensor) could not add element to the domain\n";
        cerr << "Element number: " << ElementNumber << endln;
        return -1;
    }

    return 0;

};
