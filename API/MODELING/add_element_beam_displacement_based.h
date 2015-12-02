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
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


//!
//! @mainpage
//! \n
//! <div class="contents">
//! <h1>DSL of Displacement Based Beam Element (3D)</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (September 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of displacement based beam element (3D).
//! \n
//! \n
//! For information about the function and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//!
//!


//! Inputs:
//! - ElementNumber: unique element object tag
//! - iNode , jNode: end nodes
//! - TransformationNumber: identifier for previously-defined coordinate-transformation (CrdTransf) object
//! - numberofintegrationpoints: number of integration points along the element.
//! - SectionNumber: identifier for previously-defined section object
//! - rho: element mass density (per unit length)
//! - integrationrule: integration method to be used:
//!           - Lobatto
//!           - Legendre
//!           - Radau
//!           - NewtonCotes





//! \n
//! \n
//! \n
//! <B> For information about the theory behind this function and its arguments, please consult the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/CompGeomechanicsLectureNotes/CompGeomechanicsLectureNotes.html" target="_blank">lecture notes. </A></div>
//! \n
//! \n




int add_element_beam_displacement_based(int ElementNumber,
                                        int iNode,
                                        int jNode,
                                        int numberofintegrationpoints,
                                        int SectionNumber,
                                        double rho,
                                        string integrationrule,
                                        double vecInLocXZPlane_x, double vecInLocXZPlane_y, double vecInLocXZPlane_z,
                                        double rigJntOffset1_x, double rigJntOffset1_y, double rigJntOffset1_z,
                                        double rigJntOffset2_x, double rigJntOffset2_y, double rigJntOffset2_z)
{

    Element* theElement = 0;

    SectionForceDeformation* theSection = 0;
    theSection = theDomain.getSection(SectionNumber);

    if (theSection == 0)
    {
        cerr << "WARNING: (add_element_beam_displacement_based) section not found\n";
        cerr << "Section: " << SectionNumber;
        cerr << "\ndispBeamColumn element: " << ElementNumber << endln;
        return -1;
    }



    SectionForceDeformation** sections = new SectionForceDeformation *[numberofintegrationpoints];

    for (int i = 0; i < numberofintegrationpoints; i++)
    {
        sections[i] = theSection;
    }


    const char* integrationrule_char = integrationrule.c_str();


    BeamIntegration* beamIntegr = 0;

    if (strcmp(integrationrule_char, "Lobatto") == 0)
    {
        beamIntegr = new LobattoBeamIntegration();
    }

    if (strcmp(integrationrule_char, "Legendre") == 0)
    {
        beamIntegr = new LegendreBeamIntegration();
    }

    if (strcmp(integrationrule_char, "Radau") == 0)
    {
        beamIntegr = new RadauBeamIntegration();
    }

    if (strcmp(integrationrule_char, "NewtonCotes") == 0)
    {
        beamIntegr = new NewtonCotesBeamIntegration();
    }

    //Revise!!
    // theElement = new DispBeamColumn3d(ElementNumber, iNode, jNode, numberofintegrationpoints, sections,
    //                                   *beamIntegr, rho,
    //                                   vecInLocXZPlane_x, vecInLocXZPlane_y, vecInLocXZPlane_z,
    //                                   rigJntOffset1_x,   rigJntOffset1_y,   rigJntOffset1_z,
    //                                   rigJntOffset2_x,   rigJntOffset2_y,   rigJntOffset2_z);



    // if (theElement == 0)
    // {
    //     cerr << "WARNING: (add_element_beam_displacement_based) ran out of memory creating element\n";
    //     cerr << "dispBeamColumn element: " << ElementNumber << endln;
    //     return -1;
    // }

    // if (theDomain.addElement(theElement) == false)
    // {
    //     cerr << "WARNING: (add_element_beam_displacement_based) could not add element to the domain\n";
    //     cerr << "dispBeamColumn element number: " << ElementNumber << endln;
    //     return -1;
    // }

    return 0;

};



