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

#include "RoundedMohrCoulomb.h"

//First constructor, creates a material at its "ground state" from its parameters.
RoundedMohrCoulomb::RoundedMohrCoulomb(int tag_in, double rho_in, double E_in, double nu_in, double m_in, double qa_in, double pc_in, double e_in, double H_eta_in, double eta0_in, double p0) :
    RMCBase::ClassicElastoplasticMaterial(tag_in, rho_in, p0,
                                          RMC_YFType(m_in, qa_in, pc_in, e_in, eta),    // Point YF to internal variables
                                          LinearIsotropic3D_EL(E_in, nu_in), // Create Elasticity
                                          RMC_PFType(alpha, eta),       // Point PF to the internal variables
                                          RMCVarsType(alpha, eta)),     // Declare the list of internal variables
    alpha(0.), // Set not to evolve. Dummy variable. But needed for the VM PF type which has two scalars
    eta(H_eta_in, eta0_in)
{

}

// Second constructor is not called by the user, instead it is called when creating a copy of the
// material. This must provide an initialization for the state variables and link the components
// to these variables appropriately.
RoundedMohrCoulomb::RoundedMohrCoulomb(int tag_in, double rho, double p0, RMC_YFType &yf,
                                       LinearIsotropic3D_EL &el,
                                       RMC_PFType &pf,
                                       RMCVarsType &vars) :
    RMCBase::ClassicElastoplasticMaterial(tag_in, this->getRho(),
                                          p0,     //Sets p0
                                          RMC_YFType(0, 0, 0, 0, eta), // Point YF to new internal variables
                                          LinearIsotropic3D_EL(el), // Create Elasticity -- use copy constructor here
                                          RMC_PFType(alpha, eta),    // Point PF to the internal variables
                                          RMCVarsType(alpha, eta)),   // Declare the list of internal variables
    alpha(0.),
    eta(0.)
{

}

RoundedMohrCoulomb::RoundedMohrCoulomb() :
    RMCBase::ClassicElastoplasticMaterial(0, 0, 0,
                                          RMC_YFType(0, 0, 0, 0, eta),   // Point YF to internal variables
                                          LinearIsotropic3D_EL(0, 0), // Create Elasticity
                                          RMC_PFType(alpha, eta),       // Point PF to the internal variables
                                          RMCVarsType(alpha, eta)),     // Declare the list of internal variables
    alpha(0),
    eta(0)
{

}


void
RoundedMohrCoulomb::Print(ostream& s, int flag)
{
    s << "RoundedMohrCoulomb" << endln;
    s << "\tTag: " << this->getTag() << endln;
    s << " eta =  " <<  eta << endl;
    RMCBase::Print(s);
}