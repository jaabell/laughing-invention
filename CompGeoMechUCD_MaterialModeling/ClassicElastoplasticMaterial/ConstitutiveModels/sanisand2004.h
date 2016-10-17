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
#include "../PlasticFlowDirections/sanisand2004_PF.h"

//Elasticity
#include "../ElasticityModels/sanisand2004_EL.h"

//Evolving variables
#include "../EvolvingVariables/sanisand2004_alpha_EV.h"
#include "../EvolvingVariables/sanisand2004_e_EV.h"
#include "../EvolvingVariables/sanisand2004_z_EV.h"
#include "../EvolvingVariables/sanisand2004_alpha_in_EV.h"
#include "../EvolvingVariables/LinearHardeningScalar_EV.h"




#include <classTags.h>

//Drucker Prager Model with linear hardening (sanisand2004)
class sanisand2004;  //This model we will define


//Typedefs for internal variables list, yield function, and plastic flow function
typedef MaterialInternalVariables <sanisand2004_alpha_in_EV,
        sanisand2004_alpha_EV,
        sanisand2004_z_EV,
        sanisand2004_e_EV,
        LinearHardeningScalar_EV >
        sanisand2004VarsType;

typedef DruckerPrager_YF < sanisand2004_alpha_EV,
        LinearHardeningScalar_EV>
        sanisand2004_YFType;

typedef sanisand2004_PF <sanisand2004_alpha_EV,
        sanisand2004_z_EV,
        LinearHardeningScalar_EV,
        sanisand2004_e_EV >
        sanisand2004_PFType;

//Create a helpful typedef for the base class from which we will inherit to create the n
typedef ClassicElastoplasticMaterial <sanisand2004_EL,
        sanisand2004_YFType,
        sanisand2004_PFType,
        sanisand2004VarsType,
        ND_TAG_CEM_sanisand2004,
        sanisand2004
        > sanisand2004Base;

//Define the new class. We must provide two constructor and the evolving variables as data-members.
class sanisand2004 : public sanisand2004Base
{
public:

    sanisand2004(int tag_in, double rho_, double initial_confinement_p0,
                 double G0_in, double patm_in, double nu_in,
                 double h0_in, double ch_in,
                 double Me_in, double Mc_in, double ec0_in, double lambda_c_in,
                 double xi_in,
                 double nb_in,
                 double A0_in, double cz_in, double zmax_in, double nd_in,
                 double m_in, double e_in) ;

    sanisand2004(int tag_in, double rho, double initial_confinement_p0, sanisand2004_YFType & yf,
                 sanisand2004_EL & el,
                 sanisand2004_PFType & pf,
                 sanisand2004VarsType & vars);

    sanisand2004() ;

    void Print(ostream& s, int flag);


    //The state variables.
private:
    sanisand2004_alpha_EV alpha;    // SAniSand backstress
    sanisand2004_alpha_in_EV alpha_in;    // SAniSand backstress at last reload
    sanisand2004_z_EV z;            // SAniSand fabric dilatancy tensor
    LinearHardeningScalar_EV m;     //  size of the Drucker-Prager cone (k elsewhere)
    sanisand2004_e_EV e;     //  void ratio
};

