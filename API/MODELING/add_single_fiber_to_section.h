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


int add_single_fiber_to_section(int FiberNumber,
                                int SectionNumber,
                                int MaterialNumber,
                                double Area,
                                double yLocation,
                                double zLocation)
{
    UniaxialMaterial* uniaxialmaterial = theDomain.getUniaxialMaterial(MaterialNumber);

    static Vector fiberPosition(2);
    fiberPosition(0) = yLocation;
    fiberPosition(1) = zLocation;

    Fiber* theFiber = 0;
    theFiber = new UniaxialFiber3d(FiberNumber, *uniaxialmaterial, Area, fiberPosition);

    if (theFiber == 0)
    {
        cerr.flush() <<  "add_single_fiber_to_section: WARNING unable to allocate fiber \n";
        return -1;
    }

    SectionForceDeformation *theSection = theDomain.getSection(SectionNumber);
    if (theSection == 0) {
        cerr.flush() <<  "add_single_fiber_to_section: WARNING section does not exist \n";
        return -1;
    }

    if (theSection->getClassTag() != SEC_TAG_FiberSectionGJ) {
        cerr.flush() <<  "add_single_fiber_to_section: WARNING section must be a fiber section \n";
        return -1;
    }      
    FiberSectionGJ *theFiberSection = (FiberSectionGJ *)theSection;
    theFiberSection->addFiber(*theFiber);

    return 0;
};




