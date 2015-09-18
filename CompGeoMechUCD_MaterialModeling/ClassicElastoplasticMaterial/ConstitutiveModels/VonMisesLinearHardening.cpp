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

#include "VonMisesLinearHardening.h"

//First constructor, creates a material at its "ground state" from its parameters.
VonMisesLinearHardening::VonMisesLinearHardening(int tag_in, double k0_in, double H_alpha, double H_k, double E, double nu, double rho_) :
	VMLHBase::ClassicElastoplasticMaterial(tag_in, rho_,
	                                       VMLH_YFType(alpha, k),  		// Point YF to internal variables
	                                       LinearIsotropic3D_EL(E, nu), // Create Elasticity
	                                       VMLH_PFType(alpha, k),       // Point PF to the internal variables
	                                       VMLHVarsType(alpha, k)),		// Declare the list of internal variables
	alpha(H_alpha),
	k(H_k, k0_in)
{

};

// Second constructor is not called by the user, instead it is called when creating a copy of the
// material. This must provide an initialization for the state variables and link the components
// to these variables appropriately.
VonMisesLinearHardening::VonMisesLinearHardening(int tag_in, double rho, VMLH_YFType &yf,
        LinearIsotropic3D_EL &el,
        VMLH_PFType &pf,
        VMLHVarsType &vars) :
	VMLHBase::ClassicElastoplasticMaterial(tag_in, this->getRho(),
	                                       VMLH_YFType(alpha, k),    // Point YF to new internal variables
	                                       LinearIsotropic3D_EL(el), // Create Elasticity -- use copy constructor here
	                                       VMLH_PFType(alpha, k),    // Point PF to the internal variables
	                                       VMLHVarsType(alpha, k)),   // Declare the list of internal variables
	alpha(0),
	k(0, 0)
{

};