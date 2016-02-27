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


#include "CorotCrdTransf3d.h"

int add_element_DispBeamColumn3d(int ElementNumber,
                                 int iNode,
                                 int jNode,
                                 int number_of_integration_points,
                                 int section_number,
                                 double rho,
                                 double vecInLocXZPlane_x, double vecInLocXZPlane_y, double vecInLocXZPlane_z,
                                 double rigJntOffset1_x, double rigJntOffset1_y, double rigJntOffset1_z,
                                 double rigJntOffset2_x, double rigJntOffset2_y, double rigJntOffset2_z)
{


    // for the section
    SectionForceDeformation **theSections = new SectionForceDeformation *[number_of_integration_points];
    for (short i = 0; i < number_of_integration_points; ++i)
    {
        theSections[i] = theDomain.getSection(section_number);
    }


    // check the section status before the following operation.
    if (theSections == 0)
    {
        cerr << "WARNING: (add_element_DispBeamColumn3d) section not found\n";
        cerr << "Section: " << section_number;
        cerr << "\ndispBeamColumn element: " << ElementNumber << endln;
        return -1;
    }



    // for the transformation rule from section to element
    Vector vecXZ(3);
    vecXZ(0) = vecInLocXZPlane_x;
    vecXZ(1) = vecInLocXZPlane_y;
    vecXZ(2) = vecInLocXZPlane_z;

    Vector offset1(3);
    offset1(0) = rigJntOffset1_x;
    offset1(1) = rigJntOffset1_y;
    offset1(2) = rigJntOffset1_z;

    Vector offset2(3);
    offset2(0) = rigJntOffset2_x;
    offset2(1) = rigJntOffset2_y;
    offset2(2) = rigJntOffset2_z;

    CorotCrdTransf3d *crdTransf = new CorotCrdTransf3d(1, vecXZ, offset1, offset2);


    // for a matched integration rule
    // const char* integrationrule_char = integrationrule.c_str();
    //
    BeamIntegration* beamIntegr = new LobattoBeamIntegration();
    // if (strcmp(integrationrule_char, "Lobatto") == 0){
    //     beamIntegr = new LobattoBeamIntegration();
    // }else if (strcmp(integrationrule_char, "Legendre") == 0){
    //     beamIntegr = new LegendreBeamIntegration();
    // }else if (strcmp(integrationrule_char, "Radau") == 0){
    //     beamIntegr = new RadauBeamIntegration();
    // }else if (strcmp(integrationrule_char, "NewtonCotes") == 0){
    //     beamIntegr = new NewtonCotesBeamIntegration();
    // }

    // cMass !=0 . Not lumped mass in calculation.
    int cMass = 1;
    // for the element call.
    Element* theElement = 0;
    theElement = new DispBeamColumn3d(ElementNumber, iNode, jNode, number_of_integration_points, theSections,
                                      *beamIntegr, *crdTransf, rho, cMass);
    // check the element status
    if (theElement == 0)
    {
        cerr << "WARNING: (add_element_DispBeamColumn3d) ran out of memory creating element\n";
        cerr << "dispBeamColumn element: " << ElementNumber << endln;
        return -1;
    }

    if (theDomain.addElement(theElement) == false)
    {
        cerr << "WARNING: (add_element_DispBeamColumn3d) could not add element to the domain\n";
        cerr << "element number: " << ElementNumber << endln;
        return -1;
    }

    return 0;
};



