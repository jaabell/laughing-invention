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
//! - maxNumPatches: maximum number of patches to be used for the specified section
//! - maxNumReinfLayers: maximum number of reinforcing layers to be used for the specified section



int add_section_to_model(int SectionNumber,
                         int maxNumPatches,
                         int maxNumReinfLayers)
{
    SectionRepres* fiberSectionRepr = 0;
    fiberSectionRepr = new FiberSectionRepr(SectionNumber, maxNumPatches, maxNumReinfLayers);

    if (fiberSectionRepr == NULL)
    {
        cerr << "Error: (add_section_to_model) memory allocation problem for fiberSectionRepr!" << endl;
        return -1;
    }

    if ( theDomain.addSectionRepres(*fiberSectionRepr) != 0 )
    {
        cerr << "Error: (add_section_to_model) Section " << SectionNumber << " could not be added to the domain " << endl;
        return -1;
    }

    return 0;

};




