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
//! - Knormal: stiffness in normal direction
//! - Ktangent: stiffness in tangential direction
//! - frictionRatio: Friction coefficient
//! - x_local_1, x_local_2, x_local_3: Vector components in global coordinates defining local x-axis of the element



int add_element_contact_3dof_to_3dof(int ElementNumber,
                                     int iNode,
                                     int jNode,
                                     double Knormal,
                                     double Ktangent,
                                     double frictionRatio,
                                     double x_local_1,
                                     double x_local_2,
                                     double x_local_3)
{
    Element* theElement = 0;
    theElement = new ContactElement_3DOF_3DOF(ElementNumber, iNode, jNode, Knormal, Ktangent, frictionRatio, x_local_1, x_local_2, x_local_3);

    if (theElement == NULL)
    {
        cerr << "Error: (add_element_contact_3dof_to_3dof) memory allocation problem for theElement!" << endl;
        return -1;
    }

    if (theDomain.addElement(theElement) == false)
    {
        cerr << "WARNING (add_element_contact_3dof_to_3dof) could not add element to the domain\n";
        cerr << "Element Number: " << ElementNumber << endln;
        return -1;
    }

    return 0;

};







