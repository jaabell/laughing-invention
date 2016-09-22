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

#include "CamClayLT.h"
#include "NDMaterialLT.h"
#include <iostream>
#include "../../../ltensor/LTensor.h"
#include "../ClassicElastoplasticityGlobals.h"

//First constructor, creates a material at its "ground state" from its parameters.
CamClayLT::CamClayLT(int tag_in,
                     double M_,
                     double lambda_,
                     double kappa_,
                     double e0_,
                     double p0_,
                     double nu_,
                     double initial_confinement,
                     double rho_) :
    CCBase::ClassicElastoplasticMaterial(tag_in, rho_, initial_confinement, //Initial confinement
                                         CC_YFType(M_, p0),       // Point YF to internal variables
                                         CamClay_EL(e0_, kappa_, nu_), // Create Elasticity
                                         CC_PFType(M_, p0),       // Point PF to the internal variables
                                         CCVarsType(p0)),     // Declare the list of internal variables
    p0( M_,  lambda_,  kappa_,  e0_,  p0_)
{


}

// Second constructor is not called by the user, instead it is called when creating a copy of the
// material. This must provide an initialization for the state variables and link the components
// to these variables appropriately.
CamClayLT::CamClayLT(int tag_in, double rho,
                     double pressure,
                     CC_YFType &yf,
                     CamClay_EL &el,
                     CC_PFType &pf,
                     CCVarsType &vars) :
    CCBase::ClassicElastoplasticMaterial(tag_in, this->getRho(), pressure, // Initial confinement
                                         CC_YFType(0, p0),       // Point YF to internal variables
                                         CamClay_EL(0, 0, 0), // Create Elasticity
                                         CC_PFType(0, p0),       // Point PF to the internal variables
                                         CCVarsType(p0)),     // Declare the list of internal variables
    p0( 0,  0,  0,  0,  pressure)
{
}

CamClayLT::CamClayLT() :
    CCBase::ClassicElastoplasticMaterial(0, 0, 0.0, //Initial confinement
                                         CC_YFType(0, p0),       // Point YF to internal variables
                                         CamClay_EL(0, 0, 0),  // Create Elasticity
                                         CC_PFType(0, p0),       // Point PF to the internal variables
                                         CCVarsType(p0)),     // Declare the list of internal variables
    p0(0, 0, 0, 0, 0)
{}


void
CamClayLT::Print(ostream& s, int flag)
{
    s << "CamClayLT::" << endln;
    s << "\tTag: " << this->getTag() << endln;
    s << " Please Implement Me !!! " << endl;
    // s << "\tElastic_Modulus: " << E << endln;
    // s << "\tPoissons_Ratio: " << v << endln;
    // s << "\tDensity: " << rho << endln;
    // s << "\tVon_Mises_radius: "
    // s << "\tKinematic_hardening_rate: "
    // s << "\tIsotropic_hardening_rate: "
}

