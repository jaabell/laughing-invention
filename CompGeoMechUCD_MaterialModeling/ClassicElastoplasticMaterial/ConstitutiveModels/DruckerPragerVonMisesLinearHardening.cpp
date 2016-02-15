///////////////////////////////////////////////////////////////////////////////
//   COPYLEFT (C): Woody's viral GPL-like license (by BJ):
//                 ``This    source  code is Copyrighted in
//                 U.S.,  for  an  indefinite  period,  and anybody
//                 caught  using it without our permission, will be
//                 mighty good friends of ourn, cause we don't give
//                 a  darn.  Hack it. Compile it. Debug it. Run it.
//                 Yodel  it.  Enjoy it. We wrote it, that's all we
//                 wanted to do.''
//
//
// COPYRIGHT (C):     :-))
// PROJECT:           Object Oriented Finite Element Program
// FILE:
// CLASS:
// MEMBER FUNCTIONS:
//
// MEMBER VARIABLES
//
// PURPOSE:
//
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:
// PROGRAMMER:
// DATE:
// UPDATE HISTORY:
//
//
/////////////////////////////////////////////////////////////////////////////

#include "DruckerPragerVonMisesLinearHardening.h"

//First constructor, creates a material at its "ground state" from its parameters.
DruckerPragerVonMisesLinearHardening::DruckerPragerVonMisesLinearHardening(int tag_in, double k0_in, double H_alpha, double H_k, double E, double nu, double rho_, double p0) :
    DPVMLHBase::ClassicElastoplasticMaterial(tag_in, rho_, p0,
            DPLH_YFType(alpha, k),       // Point YF to internal variables
            LinearIsotropic3D_EL(E, nu), // Create Elasticity
            VMLH_PFType(alpha, k),       // Point PF to the internal variables
            DPLHVarsType(alpha, k)),     // Declare the list of internal variables
    alpha(H_alpha),
    k(H_k, k0_in)
{

};

// Second constructor is not called by the user, instead it is called when creating a copy of the
// material. This must provide an initialization for the state variables and link the components
// to these variables appropriately.
DruckerPragerVonMisesLinearHardening::DruckerPragerVonMisesLinearHardening(int tag_in, double rho, double p0,
        DPLH_YFType &yf,
        LinearIsotropic3D_EL &el,
        VMLH_PFType &pf,
        DPLHVarsType &vars) :
    DPVMLHBase::ClassicElastoplasticMaterial(tag_in, this->getRho(),
            p0,     //Sets p0
            DPLH_YFType(alpha, k),    // Point YF to new internal variables
            LinearIsotropic3D_EL(el), // Create Elasticity -- use copy constructor here
            VMLH_PFType(alpha, k),    // Point PF to the internal variables
            DPLHVarsType(alpha, k)),   // Declare the list of internal variables
    alpha(0),
    k(0, 0)
{

};

DruckerPragerVonMisesLinearHardening::DruckerPragerVonMisesLinearHardening() :
    DPVMLHBase::ClassicElastoplasticMaterial(0, 0, 0,
            DPLH_YFType(alpha, k),       // Point YF to internal variables
            LinearIsotropic3D_EL(0, 0), // Create Elasticity
            VMLH_PFType(alpha, k),       // Point PF to the internal variables
            DPLHVarsType(alpha, k)),     // Declare the list of internal variables
    alpha(0),
    k(0, 0)
{

}

//Checks whether predicted stress is less than zero, in which case sets stress to low confinement
//value and gives a reduced stiffness.
int DruckerPragerVonMisesLinearHardening::pre_integration_callback(const DTensor2 &depsilon,
        const DTensor2 &dsigma,
        const DTensor2 &TrialStress,
        const DTensor4 &Stiffness,
        double yf1,
        double yf2,
        bool & returns)
{
    using namespace ClassicElastoplasticityGlobals;
    static DTensor2 str(3, 3, 0);
    static DTensor4 stiff(3, 3, 3, 3, 0);
    double p = -(TrialStress(0, 0) + TrialStress(1, 1) + TrialStress(2, 2)) / 3;
    if (p < 0)
    {
        str *= 0;
        stiff *= 0;
        str(0, 0) = -0.1;
        str(1, 1) = -0.1;
        str(2, 2) = -0.1;
        stiff = Stiffness;// / 10000;
        stiff *= 1. / 10000.;
        this->setTrialStress(str);

        returns = true;
    }
    else
    {
        returns = false;
    }
    return 0;
}