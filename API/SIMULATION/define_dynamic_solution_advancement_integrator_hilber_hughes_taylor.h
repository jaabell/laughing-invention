///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Nima Tafazzoli, Boris Jeremic
// DATE:               October 2009
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

//! Inputs:
//! - HHT_Alpha: HHT parameter



int define_dynamic_solution_advancement_integrator_hilber_hughes_taylor(double HHT_Alpha)
{
    double alpha = HHT_Alpha;
    theTransientIntegrator  = new HHT_alpha_method(alpha);
    //     theTransientIntegrator  = new HHT(0.9);


    //     double beta = (1.0-alpha)*(1.0-alpha)*0.25;
    //     double gamma = 0.5-alpha;
    //     theTransientIntegrator  = new HHT_alpha_method(alpha, beta, gamma);

    if (theTransientIntegrator == NULL)
    {
        cerr << "Error: (define_dynamic_solution_advancement_integrator_hilber_hughes_taylor) memory allocating theTransientIntegrator!" << endl;
        return -1;
    }

    return 0;

};



int define_dynamic_solution_advancement_integrator_hilber_hughes_taylor(double, double, double);
int define_dynamic_solution_advancement_integrator_hilber_hughes_taylor(double HHT_Alpha, double HHT_Beta, double HHT_Gamma)
{
    double alpha = HHT_Alpha;
    double beta  = HHT_Beta;
    double gamma = HHT_Gamma;
    theTransientIntegrator  = new HHT_alpha_method(alpha, beta, gamma);

    if (theTransientIntegrator == NULL)
    {
        cerr << "Error: (define_dynamic_solution_advancement_integrator_hilber_hughes_taylor) memory allocating theTransientIntegrator!" << endl;
        return -1;
    }

    return 0;

};

