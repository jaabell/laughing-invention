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
//! - SectionNumber: unique section object tag
//! - MaterialNumber: unique material object tag to be used for section
//! - numSubdivRad: number of subdivisions (fibers) in the radial direction
//! - numSubdivCirc: number of subdivisions (fibers) in the circumferential direction
//! - yCenter , zCenter: y & z coordinates of the center of the circle
//! - internalRad: internal radius
//! - externalRad: external radius
//! - startAng: starting angle
//! - endAng: ending angle




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



