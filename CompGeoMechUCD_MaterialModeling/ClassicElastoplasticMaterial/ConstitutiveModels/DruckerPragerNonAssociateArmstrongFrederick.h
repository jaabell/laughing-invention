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
//         Yield Function: Drucker-Prager
//         Plastic Flow  : Non-associate
//         Hardening type: Armstrong-Frederick
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
#include "../PlasticFlowDirections/DruckerPragerNonAssociate_PF.h"

//Elasticity
#include "../ElasticityModels/LinearIsotropic3D_EL.h"

//Evolving variables
#include "../EvolvingVariables/ArmstrongFrederickTensor_EV.h"
#include "../EvolvingVariables/LinearHardeningScalar_EV.h"



#include <classTags.h>



// New materials are created by subclassing instances of the ClassicElastoplasticMaterial<.,.,.,.,>
// template class, with the appropriate components as template parameters.
// Heavy use of templating is made, therefore typedeffing is a friend in helping clear up the mess.

//Drucker Prager Model with linear hardening (DPAF)
class DruckerPragerNonAssociateArmstrongFrederick;  //This model we will define

//Activate pre_integration_callback to handle tension case
template< >
struct supports_pre_integration_callback<DruckerPragerNonAssociateArmstrongFrederick>
{
    static const bool value = true;
};

//Typedefs for internal variables list, yield function, and plastic flow function
typedef MaterialInternalVariables < ArmstrongFrederickTensor_EV, LinearHardeningScalar_EV> DPNAAFVarsType;
typedef DruckerPrager_YF < ArmstrongFrederickTensor_EV, LinearHardeningScalar_EV> DPNAAF_YFType;
typedef DruckerPragerNonAssociate_PF < ArmstrongFrederickTensor_EV, LinearHardeningScalar_EV> DPNAAF_PFType;

//Create a helpful typedef for the base class from which we will inherit to create the n
typedef ClassicElastoplasticMaterial <LinearIsotropic3D_EL,
        DPNAAF_YFType,
        DPNAAF_PFType,
        DPNAAFVarsType,
        ND_TAG_CEM_DruckerPragerNonAssociateArmstrongFrederick,
        DruckerPragerNonAssociateArmstrongFrederick
        > DPNAAFBase;

//Define the new class. We must provide two constructor and the evolving variables as data-members.
class DruckerPragerNonAssociateArmstrongFrederick : public DPNAAFBase
{
public:

    //First constructor, creates a material at its "ground state" from its parameters.
    DruckerPragerNonAssociateArmstrongFrederick(int tag_in, double k0_in, double ha_alpha, double cr_alpha, double H_k, double E, double nu, double rho_, double p0, double xi, double Kd) ;

    // Second constructor is not called by the user, instead it is called when creating a copy of the
    // material. This must provide an initialization for the state variables and link the components
    // to these variables appropriately.
    DruckerPragerNonAssociateArmstrongFrederick(int tag_in, double rho, double p0, DPNAAF_YFType & yf,
                                    LinearIsotropic3D_EL & el,
                                    DPNAAF_PFType & pf,
                                    DPNAAFVarsType & vars);

    // Empty constructor for parallel
    DruckerPragerNonAssociateArmstrongFrederick() ;

    int pre_integration_callback(const DTensor2&, const DTensor2&, const DTensor2&, const DTensor4&, double, double, bool&);


    //The state variables.
private:
    ArmstrongFrederickTensor_EV alpha; // Backstress
    LinearHardeningScalar_EV k;     // Critical stress ratio (k = M under this formulation)
    // int HARDENING_TYPE; //Determine the hardening type according to hardening rate. This is useful for consistent stiffness tensor. 
};

