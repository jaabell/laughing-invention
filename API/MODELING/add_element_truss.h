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
//! - iNode , jNode: end nodes
//! - MaterialNumber: tag of the uniaxial material to be used
//! - sectionarea: section area of the truss element
//! - rho: density





int add_element_truss(int ElementNumber,
                      int iNode,
                      int jNode,
                      int MaterialNumber,
                      double sectionarea,
                      double rho)
{

    UniaxialMaterial* uniaxialmaterial = 0;
    uniaxialmaterial = theDomain.getUniaxialMaterial(MaterialNumber);


    if (uniaxialmaterial == NULL)
    {
        cerr << "Error: (add_element_truss) memory allocation problem for uniaxialmaterial!" << endl;
        return -1;
    }

    Element* theElement = 0;
    theElement = new Truss(ElementNumber, iNode, jNode, *uniaxialmaterial, sectionarea, rho);


    if (theElement == NULL)
    {
        cerr << "Error: (add_element_truss) memory allocation problem for theElement!" << endl;
        return -1;
    }

    if (theDomain.addElement(theElement) == false)
    {
        cerr << "WARNING: (add_element_truss) could not add element to the domain\n";
        cerr << "Element element: " << ElementNumber << endln;
        return -1;
    }

    return 0;

};





