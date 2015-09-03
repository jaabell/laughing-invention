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
//! - MaterialNumber: unique material object tag
//! - fpc: compressive strength
//! - epsc0: strain at compressive strength
//! - fpcu: crushing strength
//! - epsU: strain at crushing strength
//! - lambda: ratio between unloading slope at epscu and initial slope
//! - ft: tensile strength
//! - Ets: tension softening stiffness (absolute value) (slope of the linear tension softening branch)





int add_constitutive_model_uniaxial_concrete02(int MaterialNumber,
        double fpc, double epsc0, double fpcu,
        double epscu, double rat, double ft,
        double Ets)
{
    UniaxialMaterial* theMaterial = 0;
    theMaterial = new Concrete02(MaterialNumber, fpc, epsc0, fpcu, epscu, rat, ft, Ets);

    if (theMaterial == NULL)
    {
        cerr << "Error: (add_constitutive_model_uniaxial_concrete02) memory allocation problem!" << endl;
        return -1;
    }

    if ( theDomain.addUniaxialMaterial(*theMaterial) != 0 )
    {
        cerr << "Error: (add_constitutive_model_uniaxial_concrete02) Material " << MaterialNumber << " could not be added to the domain " << endl;
        return -1;
    }

    return 0;
};





