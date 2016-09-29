///////////////////////////////////////////////////////////////////////////////
//   COPYNEFT (C): Woody's viral GPL-like license (by BJ):
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
// FINE:
// CLASS:
// MEMBER FUNCTIONS:
//
// MEMBER VARIABNES
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
#include "../PlasticFlowDirections/DruckerPragerDeviatoric_PF.h"

//Elasticity
#include "../ElasticityModels/DuncanChang_EL.h"

//Evolving variables
#include "../EvolvingVariables/ArmstrongFrederickTensor_EV.h"
#include "../EvolvingVariables/LinearHardeningScalar_EV.h"



#include <classTags.h>



// New materials are created by subclassing instances of the ClassicElastoplasticMaterial<.,.,.,.,>
// template class, with the appropriate components as template parameters.
// Heavy use of templating is made, therefore typedeffing is a friend in helping clear up the mess.

//Drucker Prager Model with linear hardening (DPAFNE)
class DruckerPragerArmstrongFrederickNE;  //This model we will define

//Activate pre_integration_callback to handle tension case
template< >
struct supports_pre_integration_callback<DruckerPragerArmstrongFrederickNE>
{
    static const bool value = true;
};

//Typedefs for internal variables list, yield function, and plastic flow function
typedef MaterialInternalVariables < ArmstrongFrederickTensor_EV, LinearHardeningScalar_EV> DPAFNEVarsType;
typedef DruckerPrager_YF < ArmstrongFrederickTensor_EV, LinearHardeningScalar_EV> DPAFNE_YFType;
typedef DruckerPragerDeviatoric_PF < ArmstrongFrederickTensor_EV, LinearHardeningScalar_EV> DPAFNE_PFType;

//Create a helpful typedef for the base class from which we will inherit to create the n
typedef ClassicElastoplasticMaterial <DuncanChang_EL,
        DPAFNE_YFType,
        DPAFNE_PFType,
        DPAFNEVarsType,
        ND_TAG_CEM_DruckerPragerArmstrongFrederickNE,
        DruckerPragerArmstrongFrederickNE
        > DPAFNEBase;

//Define the new class. We must provide two constructor and the evolving variables as data-members.
class DruckerPragerArmstrongFrederickNE : public DPAFNEBase
{
public:

    //First constructor, creates a material at its "ground state" from its parameters.
    DruckerPragerArmstrongFrederickNE(int tag_in, double k0_in, double ha_alpha, double cr_alpha, double H_k, double K_in, double pa_in, double n_in, double sigma3_max_in, double nu, double rho_, double p0) ;

    // Second constructor is not called by the user, instead it is called when creating a copy of the
    // material. This must provide an initialization for the state variables and link the components
    // to these variables appropriately.
    DruckerPragerArmstrongFrederickNE(int tag_in, double rho, double p0, DPAFNE_YFType & yf,
                                      DuncanChang_EL & el,
                                      DPAFNE_PFType & pf,
                                      DPAFNEVarsType & vars);

    // Empty constructor for parallel
    DruckerPragerArmstrongFrederickNE() ;

    int pre_integration_callback(const DTensor2&, const DTensor2&, const DTensor2&, const DTensor4&, double, double, bool&);

    void Print(ostream& s, int flag);


    //The state variables.
private:
    ArmstrongFrederickTensor_EV alpha; // Backstress
    LinearHardeningScalar_EV k;     // Critical stress ratio (k = M under this formulation)
};

