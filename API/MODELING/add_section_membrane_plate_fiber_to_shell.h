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




int add_section_membrane_plate_fiber_to_shell(int SectionNumber, int MaterialNumber, double thickness)
{


    SectionForceDeformation* theSection = 0;
    NDMaterial* material_ref = 0;

    material_ref = theDomain.getNDMaterial(MaterialNumber);

    if (material_ref == 0)
    {
        cerr << "WARNING: (add_section_membrane_plate_fiber_to_shell) material not found!\n";
        cerr << "add_section_membrane_plate_fiber_to_shell material: " << MaterialNumber << endln;
        return -1;
    }

    theSection = new MembranePlateFiberSection(SectionNumber, thickness, *material_ref);


    if (theDomain.addSection(*theSection) < 0)
    {
        cerr << "WARNING could not add section to the domain\n";
        cerr << "Section number: " << SectionNumber << endln;
        delete theSection; // invoke the material objects destructor, otherwise mem leak
        return -1;
    }


    return 0;
};



