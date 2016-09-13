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
//! - numSubdivIJ: number of subdivisions (fibers) in the IJ direction
//! - numSubdivJK: number of subdivisions (fibers) in the JK direction
//! - yi , zi: y & z-coordinates of vertex I (local coordinate system)
//! - yk , zk: y & z-coordinates of vertex K (local coordinate system)




int add_patch_rectangle_to_section(int SectionNumber,
                                   int MaterialNumber,
                                   int numSubdivIJ,
                                   int numSubdivJK,
                                   double yi,
                                   double zi,
                                   double yk,
                                   double zk)
{

    static Matrix vertexCoords(4, 2);

    vertexCoords(0, 0) = yi;
    vertexCoords(0, 1) = zi;
    vertexCoords(2, 0) = yk;
    vertexCoords(2, 1) = zk;

    vertexCoords(1, 0) = vertexCoords(2, 0);
    vertexCoords(1, 1) = vertexCoords(0, 1);
    vertexCoords(3, 0) = vertexCoords(0, 0);
    vertexCoords(3, 1) = vertexCoords(2, 1);


    SectionRepres* sectionRepres = theDomain.getSectionRepres(SectionNumber);

    if (sectionRepres == 0)
    {
        cerr <<  "WARNING: (add_patch_rectangle_to_section) cannot retrieve section\n";
        return -1;
    }

    FiberSectionRepr* fiberSectionRepr = (FiberSectionRepr*) sectionRepres;


    if (fiberSectionRepr == 0)
    {
        cerr <<  "WARNING: (add_patch_rectangle_to_section) cannot alocate fiberSectionRepr\n";
        return -1;
    }

    // create patch
    QuadPatch* patch = new QuadPatch(MaterialNumber, numSubdivIJ, numSubdivJK, vertexCoords);

    if (!patch)
    {
        cerr <<  "WARNING: (add_patch_rectangle_to_section) cannot alocate patch\n";
        return -1;
    }


    // add patch to section representation
    int error = fiberSectionRepr->addPatch(*patch);
    delete patch;

    if (error)
    {
        cerr <<  "WARNING: (add_patch_rectangle_to_section) cannot add patch to section\n";
        return -1;
    }

    return 0;
};






