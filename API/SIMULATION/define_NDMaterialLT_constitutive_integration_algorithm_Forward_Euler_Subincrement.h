///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Nima Tafazzoli, Boris Jeremic
// DATE:               Jan  2010
// UPDATE HISTORY:     Full update history in git repository.
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


int define_NDMaterialLT_constitutive_integration_algorithm_Forward_Euler_Subincrement(int algorithm, int num_of_subincrement)
{
    int errorflag = -1;

    // theDomain.setConstitutiveIntegrationMethod(algorithm, f_relative_tol, stress_relative_tol, num_of_subincrement);
    theDomain.setConstitutiveIntegrationMethod(algorithm, 0.0, 0.0, num_of_subincrement);
//     cout << "define_NDMaterialLT_constitutive_integration_algorithm() - Setting method to " << algorithm << endl;

//     if ((errorflag = NDMaterialLT::set_constitutive_integration_method(algorithm, f_relative_tol, stress_relative_tol, num_of_subincrement)))
//     {
//         errorflag = 0;
//     }
//     else
//     {
//         cerr << "define_NDMaterialLT_constitutive_integration_algorithm() - Error. Constitutive algorithm not available.\n";
//     }

// #ifdef _PARALLEL_PROCESSING
//     SubdomainIter &theSubdomains = theDomain.getSubdomains();
//     Subdomain *theSub = 0;

//     while ((theSub = theSubdomains()) != 0)
//     {
//         theSub->setConstitutiveIntegrationMethod(algorithm, f_relative_tol, stress_relative_tol, num_of_subincrement);
//     }
// #endif

    return errorflag;
};






