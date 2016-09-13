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

#include "DruckerPragerArmstrongFrederick.h"

//First constructor, creates a material at its "ground state" from its parameters.
DruckerPragerArmstrongFrederick::DruckerPragerArmstrongFrederick(int tag_in, double k0_in, double ha_alpha, double cr_alpha, double H_k, double E, double nu, double rho_, double p0) :
    DPAFBase::ClassicElastoplasticMaterial(tag_in, rho_, p0,
                                           DPAF_YFType(alpha, k),       // Point YF to internal variables
                                           LinearIsotropic3D_EL(E, nu), // Create Elasticity
                                           DPAF_PFType(alpha, k),       // Point PF to the internal variables
                                           DPAFVarsType(alpha, k)),     // Declare the list of internal variables
    alpha(ha_alpha, cr_alpha),
    k(H_k, k0_in)
{

}

// Second constructor is not called by the user, instead it is called when creating a copy of the
// material. This must provide an initialization for the state variables and link the components
// to these variables appropriately.
DruckerPragerArmstrongFrederick::DruckerPragerArmstrongFrederick(int tag_in, double rho, double p0, DPAF_YFType &yf,
        LinearIsotropic3D_EL &el,
        DPAF_PFType &pf,
        DPAFVarsType &vars) :
    DPAFBase::ClassicElastoplasticMaterial(tag_in, this->getRho(),
                                           p0,     //Sets p0
                                           DPAF_YFType(alpha, k),    // Point YF to new internal variables
                                           LinearIsotropic3D_EL(el), // Create Elasticity -- use copy constructor here
                                           DPAF_PFType(alpha, k),    // Point PF to the internal variables
                                           DPAFVarsType(alpha, k)),   // Declare the list of internal variables
    alpha(0, 0.0),
    k(0, 0)
{

}

DruckerPragerArmstrongFrederick::DruckerPragerArmstrongFrederick() :
    DPAFBase::ClassicElastoplasticMaterial(0, 0, 0,
                                           DPAF_YFType(alpha, k),       // Point YF to internal variables
                                           LinearIsotropic3D_EL(0, 0), // Create Elasticity
                                           DPAF_PFType(alpha, k),       // Point PF to the internal variables
                                           DPAFVarsType(alpha, k)),     // Declare the list of internal variables
    alpha(0, 0),
    k(0, 0)
{

}

//Checks whether predicted stress is less than zero, in which case sets stress to low confinement
//value and gives a reduced stiffness.
int DruckerPragerArmstrongFrederick::pre_integration_callback(const DTensor2 &depsilon,
        const DTensor2 &dsigma,
        const DTensor2 &TrialStress,
        const DTensor4 &Stiffness,
        double yf1,
        double yf2,
        bool & returns)
{
    using namespace ClassicElastoplasticityGlobals;
    static DTensor2 stress(3, 3, 0);
    static DTensor2 plasticstrain(3, 3, 0);
    static DTensor4 stiff(3, 3, 3, 3, 0);
    double p = -(TrialStress(0, 0) + TrialStress(1, 1) + TrialStress(2, 2)) / 3;
    if (p < 0)
    {
        stress *= 0;
        stiff *= 0;
        stress(0, 0) = -0.1;
        stress(1, 1) = -0.1;
        stress(2, 2) = -0.1;
        stiff = Stiffness;// / 10000;
        double mu = stiff(0, 1, 0, 1);
        double mu_reduced = mu / 10000;

        //Reduce only the shear components of the stiffness tensor.
        stiff( 0, 0, 0, 0 ) += -2 * mu + 2 * mu_reduced; // lambda + 2 * mu;
        // stiff( 0, 0, 1, 1 ) += -mu + mu_reduced;// lambda;
        // stiff( 0, 0, 2, 2 ) += -mu + mu_reduced;// lambda;
        stiff( 0, 1, 0, 1 ) += -mu + mu_reduced;// mu;
        stiff( 0, 1, 1, 0 ) += -mu + mu_reduced;// mu;
        stiff( 0, 2, 0, 2 ) += -mu + mu_reduced;// mu;
        stiff( 0, 2, 2, 0 ) += -mu + mu_reduced;// mu;
        stiff( 1, 0, 0, 1 ) += -mu + mu_reduced;// mu;
        stiff( 1, 0, 1, 0 ) += -mu + mu_reduced;// mu;
        // stiff( 1, 1, 0, 0 ) += -mu + mu_reduced;// lambda;
        stiff( 1, 1, 1, 1 ) += -2 * mu + 2 * mu_reduced; // lambda + 2 * mu;
        // stiff( 1, 1, 2, 2 ) += -mu + mu_reduced;// lambda;
        stiff( 1, 2, 1, 2 ) += -mu + mu_reduced;// mu;
        stiff( 1, 2, 2, 1 ) += -mu + mu_reduced;// mu;
        stiff( 2, 0, 0, 2 ) += -mu + mu_reduced;// mu;
        stiff( 2, 0, 2, 0 ) += -mu + mu_reduced;// mu;
        stiff( 2, 1, 1, 2 ) += -mu + mu_reduced;// mu;
        stiff( 2, 1, 2, 1 ) += -mu + mu_reduced;// mu;
        // stiff( 2, 2, 0, 0 ) += -mu + mu_reduced;// lambda;
        // stiff( 2, 2, 1, 1 ) += -mu + mu_reduced;// lambda;
        stiff( 2, 2, 2, 2 ) += -2 * mu + 2 * mu_reduced; // lambda + 2 * mu;


        plasticstrain = this->getCommittedPlasticStrainTensor();
        plasticstrain(i, j) = plasticstrain(i, j) + depsilon(i, j);

        this->setTrialStress(stress);
        this->setStiffness(stiff);
        this->setTrialPlastic_Strain(depsilon);

        //Reuse the 'stress' variable now to reset the backstress to zero
        stress *= 0;
        alpha.setVar(stress);
        alpha.commit_tmp();

        // cout << "pre_integration_callback acted!\n";

        returns = true;
    }
    else
    {
        returns = false;
    }
    return 0;
}

void
DruckerPragerArmstrongFrederick::Print(ostream& s, int flag)
{
    s << "DruckerPragerArmstrongFrederick" << endln;
    s << "\tTag: " << this->getTag() << endln;
    s << " Please Implement Me !!! " << endl;
}