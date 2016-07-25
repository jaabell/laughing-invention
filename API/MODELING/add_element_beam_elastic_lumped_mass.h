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
//! - A: area
//! - E: Young's modulus
//! - G: Shear Modulus
//! - Jx: polar moment of inertia
//! - Iy: moment of inertia around y
//! - Iz: moment of inertia around z
//! - iNode , jNode: end nodes
//! - rho: density
//! - sectionTag: identifier for previously-defined section object



int add_element_beam_elastic_lumped_mass(int ElementNumber,
        double A,
        double E,
        double G,
        double Jx,
        double Iy,
        double Iz,
        int iNode,
        int jNode,
        double rho,
        double vecxzPlane_X, double vecxzPlane_Y, double vecxzPlane_Z,
        double jntOffsetI_X, double jntOffsetI_Y, double jntOffsetI_Z,
        double jntOffsetJ_X, double jntOffsetJ_Y, double jntOffsetJ_Z)
{

    int SectionNumber = 0;


    Element* theElement = 0;
    theElement = new ElasticBeamLumpedMass(ElementNumber, A, E, G, Jx, Iy, Iz, iNode, jNode, rho, SectionNumber,
                                           vecxzPlane_X, vecxzPlane_Y, vecxzPlane_Z,
                                           jntOffsetI_X, jntOffsetI_Y, jntOffsetI_Z,
                                           jntOffsetJ_X, jntOffsetJ_Y, jntOffsetJ_Z);

    if (theElement == 0)
    {
        cerr << "WARNING: (add_element_beam_elastic_lumped_mass) ran out of memory creating element\n";
        cerr << "elastic beam element: " << ElementNumber << endln;
        return -1;
    }


    if (theDomain.addElement(theElement) == false)
    {
        cerr << "WARNING: (add_element_beam_elastic_lumped_mass) could not add element to the domain\n";
        cerr << "elastic beam number: " << ElementNumber << endln;
        return -1;
    }

    return 0;
};


