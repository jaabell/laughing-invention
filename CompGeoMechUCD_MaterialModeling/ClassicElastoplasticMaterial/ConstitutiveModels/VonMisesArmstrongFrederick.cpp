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

#include "VonMisesArmstrongFrederick.h"
#include "NDMaterialLT.h"
#include <iostream>
#include "../../../ltensor/LTensor.h"
#include "../ClassicElastoplasticityGlobals.h"
//First constructor, creates a material at its "ground state" from its parameters.
VonMisesArmstrongFrederick::VonMisesArmstrongFrederick(int tag_in, double k0_in, double ha_alpha, double cr_alpha, double H_k, double E, double nu, double rho_) :
    VMAFBase::ClassicElastoplasticMaterial(tag_in, rho_, 0.0, //Initial confinement can be 0 for this model
                                           VMAF_YFType(alpha, k),       // Point YF to internal variables
                                           LinearIsotropic3D_EL(E, nu), // Create Elasticity
                                           VMAF_PFType(alpha, k),       // Point PF to the internal variables
                                           VMAFVarsType(alpha, k)),     // Declare the list of internal variables
    alpha(ha_alpha, cr_alpha),
    k(H_k, k0_in)
{
    // cout << "First VMAF ctor" << endl;
    // cout << "tag_in  = " << tag_in << endl;
    // cout << "k0_in  = " << k0_in << endl;
    // cout << "H_alpha  = " << H_alpha << endl;
    // cout << "H_k  = " << H_k << endl;
    // cout << "E  = " << E << endl;
    // cout << "nu  = " << nu << endl;
    // cout << "rho_  = " << rho_ << endl;
    // cout << "yf  = " << yf(getStressTensor()) << endl;

}

// Second constructor is not called by the user, instead it is called when creating a copy of the
// material. This must provide an initialization for the state variables and link the components
// to these variables appropriately.
VonMisesArmstrongFrederick::VonMisesArmstrongFrederick(int tag_in, double p0, double rho, VMAF_YFType &yf,
        LinearIsotropic3D_EL &el,
        VMAF_PFType &pf,
        VMAFVarsType &vars) :
    VMAFBase::ClassicElastoplasticMaterial(tag_in, this->getRho(), p0, // Initial confinement can be 0 for this model
                                           VMAF_YFType(alpha, k),     // Point YF to new internal variables
                                           LinearIsotropic3D_EL(el),  // Create Elasticity -- use copy constructor here
                                           VMAF_PFType(alpha, k),     // Point PF to the internal variables
                                           VMAFVarsType(alpha, k)),   // Declare the list of internal variables
    alpha(0.0,0.0),
    k(0.0, 0.0)
{
}

VonMisesArmstrongFrederick::VonMisesArmstrongFrederick() :
    VMAFBase::ClassicElastoplasticMaterial(0, 0, 0.0, //Initial confinement can be 0 for this model
                                           VMAF_YFType(alpha, k),       // Point YF to internal variables
                                           LinearIsotropic3D_EL(0, 0),  // Create Elasticity
                                           VMAF_PFType(alpha, k),       // Point PF to the internal variables
                                           VMAFVarsType(alpha, k)),     // Declare the list of internal variables
    alpha(0.0,0.0),
    k(0.0,0.0)
{}
