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
//! - Algorithm: Explicit (=0) or Implicit (=1)
//! - rho: density
//! - E: Elastic modulus
//! - v: Poisson's ratio
//! - k:
//! - H:
//! - initialconfiningstress: initial confining pressure (possitive for compression)





int add_constitutive_model_NDMaterial_vonmises_isotropic_hardening(int MaterialNumber,
        int Algorithm,
        double rho,
        double E,
        double v,
        double k,
        double H,
        double initialconfiningstress,
        int number_of_subincrements,
        int maximum_number_of_iterations,
        double tolerance_1,
        double tolerance_2)
{
    stresstensor zeroT;
    stresstensor initStress;
    initStress.val(1, 1) = -initialconfiningstress;
    initStress.val(2, 2) = -initialconfiningstress;
    initStress.val(3, 3) = -initialconfiningstress;

    double MC[5] = {rho, E, v, k, H};
    double IS[1] = {k};
    MaterialParameter matpar(MC, 5, IS, 1);
    Isotropic_Elastic le(2, 3, initStress);
    VM_YF dpy(1, 1);
    VM_PF dpf(0, 4);
    //VM_PF dpf(-1,4);
    Linear_Eeq Eep(5);
    ScalarEvolution* SE = {&Eep};

    NDMaterial* theMaterial = 0;
    theMaterial = new NewTemplate3Dep(MaterialNumber, &matpar, &le, &dpy, &dpf, &SE, Algorithm, number_of_subincrements, maximum_number_of_iterations, tolerance_1, tolerance_2);

    if (theMaterial == NULL)
    {
        cerr << "Error: (add_constitutive_model_NDMaterial_vonmises_isotropic_hardening) memory allocation problem!" << endl;
        return -1;
    }

    if ( theDomain.addNDMaterial(*theMaterial) != 0 )
    {
        cerr << "Error: (add_constitutive_model_NDMaterial_vonmises_isotropic_hardening) Material " << MaterialNumber << " could not be added to the domain " << endl;
        return -1;
    }

    return 0;
};



