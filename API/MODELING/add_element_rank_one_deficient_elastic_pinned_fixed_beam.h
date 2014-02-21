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
// PURPOSE:           Adding rank one deficient elastic pinned fixed beam
//
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:
// PROGRAMMER:        Nima Tafazzoli, Boris Jeremic
//
// DATE:              December 2012
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////



//! \n
//! \n
//! \n
//! <B> For information about the theory behind this function and its arguments, please consult the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/CompGeomechanicsLectureNotes/CompGeomechanicsLectureNotes.html" target="_blank">lecture notes. </A></div>
//! \n
//! \n




int add_element_rank_one_deficient_elastic_pinned_fixed_beam(int ElementNumber,
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
    theElement = new rank_one_deficient_elastic_pinned_fixed_beam(ElementNumber, A, E, G, Jx, Iy, Iz, iNode, jNode, rho, SectionNumber,
            vecxzPlane_X, vecxzPlane_Y, vecxzPlane_Z,
            jntOffsetI_X, jntOffsetI_Y, jntOffsetI_Z,
            jntOffsetJ_X, jntOffsetJ_Y, jntOffsetJ_Z);

    if (theElement == 0)
    {
        cerr << "WARNING: (add_element_rank_one_deficient_elastic_pinned_fixed_beam) ran out of memory creating element\n";
        cerr << "elastic beam element: " << ElementNumber << endln;
        return -1;
    }


    if (theDomain.addElement(theElement) == false)
    {
        cerr << "WARNING: (add_element_rank_one_deficient_elastic_pinned_fixed_beam) could not add element to the domain\n";
        cerr << "elastic beam number: " << ElementNumber << endln;
        return -1;
    }

    return 0;

};


