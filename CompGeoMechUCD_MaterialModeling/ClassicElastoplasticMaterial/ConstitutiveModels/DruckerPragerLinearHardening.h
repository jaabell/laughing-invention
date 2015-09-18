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


#include "../ClassicElastoplasticityGlobals.h"
#include "../ClassicElastoplasticMaterial.h"
#include "../MaterialInternalVariables.h"

//Yield Functions
#include "../YieldFunctions/DruckerPrager_YF.h"

//Plastic flow directions
#include "../PlasticFlowDirections/DruckerPrager_PF.h"

//Elasticity Models
#include "../ElasticityModels/LinearIsotropic3D_EL.h"

//Evolving variables
#include "../EvolvingVariables/LinearHardeningTensor_EV.h"
#include "../EvolvingVariables/LinearHardeningScalar_EV.h"



#include <classTags.h>

// New materials are created by subclassing instances of the ClassicElastoplasticMaterial<.,.,.,.,>
// template class, with the appropriate components as template parameters.
// Heavy use of templating is made, therefore typedeffing is a friend in helping clear up the mess.

//Drucker Prager Model with linear hardening (DPLH)
class DruckerPragerLinearHardening;  //This model we will define

//Typedefs for internal variables list, yield function, and plastic flow function
typedef MaterialInternalVariables < LinearHardeningTensor_EV, LinearHardeningScalar_EV> DPLHVarsType;
typedef DruckerPrager_YF < LinearHardeningTensor_EV, LinearHardeningScalar_EV> DPLH_YFType;
typedef DruckerPrager_PF < LinearHardeningTensor_EV, LinearHardeningScalar_EV> DPLH_PFType;

//Create a helpful typedef for the base class from which we will inherit to create the new material.
typedef ClassicElastoplasticMaterial <LinearIsotropic3D_EL,
        DPLH_YFType,
        DPLH_PFType,
        DPLHVarsType,
        ND_TAG_CEM_DruckerPragerLinearHardening,
        DruckerPragerLinearHardening > DPLHBase;

//Define the new class. We must provide two constructor and the evolving variables as data-members.
class DruckerPragerLinearHardening : public DPLHBase
{
public:

	//First constructor, creates a material at its "ground state" from its parameters.
	DruckerPragerLinearHardening(int tag_in, double k0_in, double H_alpha, double H_k, double E, double nu, double rho_) ;

	// Second constructor is not called by the user, instead it is called when creating a copy of the
	// material. This must provide an initialization for the state variables and link the components
	// to these variables appropriately.
	DruckerPragerLinearHardening(int tag_in, double rho, DPLH_YFType & yf,
	                             LinearIsotropic3D_EL & el,
	                             DPLH_PFType & pf,
	                             DPLHVarsType & vars);

	//The state variables.
private:
	LinearHardeningTensor_EV alpha;	// Backstress
	LinearHardeningScalar_EV k;		// Critical stress ratio (k = M under this formulation)
};

