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
//! <h1>DSL of Section Patches (Circle)</h1>
//! <h3 align="center">By: <a href="http://cml00.engr.ucdavis.edu/~ntafazzoli" target="_blank">Nima Tafazzoli</A> and <a href="http://sokocalo.engr.ucdavis.edu/~jeremic" target="_blank">Boris Jeremic</A> (September 2009) </h3><br>
//! \n
//! This documentation is the API for Domain Specific Language (DSL) of circular type of section patches.
//! \n
//! \n
//! For information about the function and inputs <A HREF="a00001.html">click here! </A></div>
//! \n
//!
//!


//! Inputs:
//! - SectionNumber: unique section object tag
//! - MaterialNumber: unique material object tag to be used for section
//! - numSubdivRad: number of subdivisions (fibers) in the radial direction
//! - numSubdivCirc: number of subdivisions (fibers) in the circumferential direction
//! - yCenter , zCenter: y & z coordinates of the center of the circle
//! - internalRad: internal radius
//! - externalRad: external radius
//! - startAng: starting angle
//! - endAng: ending angle




//! \n
//! \n
//! \n
//! <B> For information about the theory behind this function and its arguments, please consult the <A HREF="http://sokocalo.engr.ucdavis.edu/~jeremic/CG/CompGeomechanicsLectureNotes/CompGeomechanicsLectureNotes.html" target="_blank">lecture notes. </A></div>
//! \n
//! \n




int add_patch_circle_to_section(int SectionNumber,
                                int MaterialNumber,
                                int numSubdivRad,
                                int numSubdivCirc,
                                double yCenter,
                                double zCenter,
                                double internalRad,
                                double externalRad,
                                double startAng,
                                double endAng)
{

    static Vector centerPosition(2);
    centerPosition(0) = yCenter;
    centerPosition(1) = zCenter;


    SectionRepres* sectionRepres = theDomain.getSectionRepres(SectionNumber);

    if (sectionRepres == 0)
    {
        cerr <<  "WARNING: (add_patch_circle_to_section) cannot retrieve section\n";
        return -1;
    }

    FiberSectionRepr* fiberSectionRepr = (FiberSectionRepr*) sectionRepres;


    if (fiberSectionRepr == 0)
    {
        cerr <<  "WARNING: (add_patch_circle_to_section) cannot alocate fiberSectionRepr\n";
        return -1;
    }

    // create patch
    CircPatch* patch = new CircPatch(MaterialNumber, numSubdivCirc, numSubdivRad,
                                     centerPosition, internalRad, externalRad,
                                     startAng, endAng);

    if (!patch)
    {
        cerr <<  "WARNING: (add_patch_circle_to_section) cannot alocate patch\n";
        return -1;
    }

    // add patch to section
    int error = fiberSectionRepr->addPatch(*patch);
    delete patch;

    if (error)
    {
        cerr <<  "WARNING: (add_patch_circle_to_section) cannot add patch to section\n";
        return -1;
    }

    return 0;
};



