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
//! - MaterialNumber: tag of the ND material to be used
//! - ElasticModulus: elastic modulus
//! - nu: Poisson's Ratio
//! - rho: density



int add_constitutive_model_NDMaterial_two_stage_equivalent_linear_elastic_isotropic_3d(int tag, double nu, double rho,
        double K, double Kur, double n, double c,
        double phi0, double dphi, double Rf, double K0,
        double Kb, double m, double pa)
{
    NDMaterial* theMaterial = 0;
    theMaterial = new TwoStageEquivalentElasticIsotropic3D(tag,  nu,  rho,
            K,  Kur,  n,  c,
            phi0,  dphi,  Rf,  K0,
            Kb,  m,  pa);

    if (theMaterial == NULL)
    {
        cerr << "Error: (add_constitutive_model_NDMaterial_linear_elastic_isotropic_3d) memory allocation problem!" << endl;
        return -1;
    }

    if ( theDomain.addNDMaterial(*theMaterial) != 0 )
    {
        cerr << "Error: (add_constitutive_model_NDMaterial_linear_elastic_isotropic_3d) Material " << tag << " could not be added to the domain " << endl;
        return -1;
    }

    return 0;
};





