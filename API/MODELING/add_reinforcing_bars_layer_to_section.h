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
//! - SectionNumber: integer tag of the section
//! - MaterialNumber: material integer tag of the previously-defined UniaxialMaterial object used to represent the stress-strain for the area of the fiber
//! - numReinfBars: number of reinforcing bars along layer
//! - reinfBarArea: area of individual reinforcing bar
//! - yStartPt, zStartPt: y and z-coordinates of starting point of reinforcing layer (local coordinate system)
//! - yEndPt, zEndPt: y and z-coordinates of ending point of reinforcing layer (local coordinate system)


int add_reinforcing_bars_layer_to_section(int SectionNumber,
        int MaterialNumber,
        int numReinfBars,
        double reinfBarArea,
        double yStartPt,
        double zStartPt,
        double yEndPt,
        double zEndPt)
{

    SectionRepres* sectionRepres = theDomain.getSectionRepres(SectionNumber);

    if (sectionRepres == 0)
    {
        cerr <<  "WARNING: (add_reinforcing_bars_layer_to_section) cannot retrieve section\n";
        return -1;
    }


    FiberSectionRepr* fiberSectionRepr = (FiberSectionRepr*) sectionRepres;

    if (fiberSectionRepr == 0)
    {
        cerr <<  "WARNING: (add_reinforcing_bars_layer_to_section) cannot alocate fiberSectionRepr\n";
        return -1;
    }


    // create the reinforcing layer
    static Vector startPt(2);
    static Vector endPt(2);

    startPt(0)  = yStartPt;
    startPt(1)  = zStartPt;
    endPt(0) = yEndPt;
    endPt(1) = zEndPt;

    StraightReinfLayer* reinfLayer = new StraightReinfLayer (MaterialNumber,
            numReinfBars, reinfBarArea,
            startPt, endPt);

    if (!reinfLayer)
    {
        cerr <<  "WARNING: (add_reinforcing_bars_layer_to_section) cannot alocate reinfLayer\n";
        return -1;
    }


    // add reinfLayer to section
    int error = fiberSectionRepr->addReinfLayer(*reinfLayer);
    delete reinfLayer;

    if (error)
    {
        cerr <<  "WARNING: (add_reinforcing_bars_layer_to_section) cannot add reinforcing layer to section\n";
        return -1;
    }

    return 0;
};


