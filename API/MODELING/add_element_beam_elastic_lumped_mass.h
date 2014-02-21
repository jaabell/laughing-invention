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
// DATE:              May 2009
// UPDATE HISTORY:    October 2009
//
///////////////////////////////////////////////////////////////////////////////



//!
//! @mainpage
//! \n
//! <div class="contents">
//! <h1>DSL of Elastic Beam Element (3D)</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (October 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of elastic beam element (3D).
//! \n
//! \n
//! For information about the function and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//!
//!


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




//! \n
//! \n
//! \n
//! <B> For information about the theory behind this function and its arguments, please consult the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/CompGeomechanicsLectureNotes/CompGeomechanicsLectureNotes.html" target="_blank">lecture notes. </A></div>
//! \n
//! \n




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


