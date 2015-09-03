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
//! - fy: yield strength
//! - E0: initial elastic tangent
//! - b: strain-hardening ratio (ratio between post-yield tangent and initial elastic tangent)
//! - R0, cR1, cR2: control the transition from elastic to plastic branches. Recommended values: R0=between 10 and 20, cR1=0.925, cR2=0.15
//! - a1, a2, a3, a4: isotropic hardening parameters
//! - a1: isotropic hardening parameter, increase of compression yield envelope as proportion of yield strength after a plastic strain of a2*(Fy/E).
//! - a2: isotropic hardening parameter (see explanation under a1)
//! - a3: isotropic hardening parameter, increase of tension yield envelope as proportion of yield strength after a plastic strain of a4*(Fy/E)
//! - a4: isotropic hardening parameter (see explanation under a3)





int add_constitutive_model_uniaxial_steel02(int MaterialNumber,
        double fy, double E0, double b,
        double R0, double cR1, double cR2,
        double a1, double a2, double a3, double a4)


{
    UniaxialMaterial* theMaterial = 0;
    theMaterial = new Steel02(MaterialNumber, fy, E0, b, R0, cR1, cR2, a1, a2, a3, a4);

    if (theMaterial == NULL)
    {
        cerr << "Error: (add_constitutive_model_uniaxial_steel02) memory allocation problem!" << endl;
        return -1;
    }

    if ( theDomain.addUniaxialMaterial(*theMaterial) != 0 )
    {
        cerr << "Error: (add_constitutive_model_uniaxial_steel02) Material " << MaterialNumber << " could not be added to the domain " << endl;
        return -1;
    }

    return 0;

};




