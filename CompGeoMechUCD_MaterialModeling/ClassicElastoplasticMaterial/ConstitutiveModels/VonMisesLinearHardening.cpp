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
#include "NDMaterialLT.h"
#include <iostream>
#include "../../../ltensor/LTensor.h"
#include "../ClassicElastoplasticityGlobals.h"
//First constructor, creates a material at its "ground state" from its parameters.
VonMisesLinearHardening::VonMisesLinearHardening(int tag_in, double k0_in, double H_alpha, double H_k, double E, double nu, double rho_) :
    VMLHBase::ClassicElastoplasticMaterial(tag_in, rho_, 0.0, //Initial confinement can be 0 for this model
                                           VMLH_YFType(alpha, k),       // Point YF to internal variables
                                           LinearIsotropic3D_EL(E, nu), // Create Elasticity
                                           VMLH_PFType(alpha, k),       // Point PF to the internal variables
                                           VMLHVarsType(alpha, k)),     // Declare the list of internal variables
    alpha(H_alpha),
    k(H_k, k0_in)
{
    // cout << "First VMLH ctor" << endl;
    // cout << "tag_in  = " << tag_in << endl;
    // cout << "k0_in  = " << k0_in << endl;
    // cout << "H_alpha  = " << H_alpha << endl;
    // cout << "H_k  = " << H_k << endl;
    // cout << "E  = " << E << endl;
    // cout << "nu  = " << nu << endl;
    // cout << "rho_  = " << rho_ << endl;
    // cout << "yf  = " << yf(getStressTensor()) << endl;

  // // ====================================
  // // working on consistent_stiffness_, not finished yet.
  // // ====================================
  // global variable. should be optimized later.
  // int HARDENING_TYPE=0;
  // if( abs(H_alpha)< MACHINE_EPSILON &&  abs(H_k)<MACHINE_EPSILON){
  //   HARDENING_TYPE = Perfectly_Plastic;
  // }else if(abs(H_alpha)<MACHINE_EPSILON &&  abs(H_k)>MACHINE_EPSILON){
  //   HARDENING_TYPE = One_Isotropic_Hardening_Only;
  // }else if(abs(H_alpha)>MACHINE_EPSILON &&  abs(H_k)<MACHINE_EPSILON){
  //   HARDENING_TYPE = One_Kinematic_Hardening_Only;
  // }else{
  //   HARDENING_TYPE = Both_One_Isotropic_One_Kinematic_Hardening;
  // }

}

// Second constructor is not called by the user, instead it is called when creating a copy of the
// material. This must provide an initialization for the state variables and link the components
// to these variables appropriately.
VonMisesLinearHardening::VonMisesLinearHardening(int tag_in, double p0, double rho, VMLH_YFType &yf,
        LinearIsotropic3D_EL &el,
        VMLH_PFType &pf,
        VMLHVarsType &vars) :
    VMLHBase::ClassicElastoplasticMaterial(tag_in, this->getRho(), p0, // Initial confinement can be 0 for this model
                                           VMLH_YFType(alpha, k),     // Point YF to new internal variables
                                           LinearIsotropic3D_EL(el),  // Create Elasticity -- use copy constructor here
                                           VMLH_PFType(alpha, k),     // Point PF to the internal variables
                                           VMLHVarsType(alpha, k)),   // Declare the list of internal variables
    alpha(0),
    k(0, 0)
{
}

VonMisesLinearHardening::VonMisesLinearHardening() :
    VMLHBase::ClassicElastoplasticMaterial(0, 0, 0.0, //Initial confinement can be 0 for this model
                                           VMLH_YFType(alpha, k),       // Point YF to internal variables
                                           LinearIsotropic3D_EL(0, 0),  // Create Elasticity
                                           VMLH_PFType(alpha, k),       // Point PF to the internal variables
                                           VMLHVarsType(alpha, k)),     // Declare the list of internal variables
    alpha(0),
    k(0, 0)
{}

// ====================================
// // working on consistent_stiffness_, not finished yet.
// // ====================================
// int VonMisesLinearHardening::consistent_stiffness_(DTensor2 const &dlambda_,
//                           DTensor2 const &sigma_,
//                           DTensor2 const &n_,
//                           DTensor2 const &m_,
//                           DTensor2 const &z_,
//                           DTensor2 const &alpha_,
//                           double   const &k_,
//                           DTensor4       &Stiffness_ ){
//   using namespace ClassicElastoplasticityGlobals;
  
//   static DTensor4 I4(3,3,3,3,0.0);
//   I4=kronecker_delta(i,k)*kronecker_delta(j,l);
//   static DTensor4 dm_dsigma(3,3,3,3,0.0);
//   dm_dsigma=m_(k,l)/sigma_(m,n);

//   switch(hardening_type){
//     case Perfectly_Plastic:{
//       cout<<"VonMisesLinearHardening::consistent_stiffness_ hardening_type Perfectly_Plastic not implemented yet!";
//       // static Dtensor4 General_C_inv(3,3,3,3,0.0);
//       // General_C_inv=I4(i,j,k,l) + dlambda_ * Stiffness_(i,j,k,l)*dm_dsigma(k,l,m,n);
//       // static Dtensor4 General_C(3,3,3,3,0.0);
//       // General_C=General_C_inv.Inv();
//       // double denominator=n_(p, q) * Stiffness_(p, q, r, s) * m_(r, s);
//       // Stiffness_(i,j,k,l)*=(General_C(i,j,k,l)-(General_C(i, j, p, q) * m_(p, q)) * (n_(r, s) * General_C(r, s, k, l) ) )/denominator
//       break;
//     }
//     case Isotropic_Hardening_Only:{
//       cout<<"VonMisesLinearHardening::consistent_stiffness_ hardening_type Isotropic_Hardening_Only not implemented yet!";
//       break;
//     }
//     case Kinematic_Hardening_Only:{

//       break;
//     }
//     case Both_Isotropic_Kinematic_Hardening:{
//       cout<<"VonMisesLinearHardening::consistent_stiffness_ hardening_type Both_Isotropic_Kinematic_Hardening not implemented yet!";
//       break;
//     }
//     default:{
//       cout<<"Stiffness_(i,j,k,l) \n";

//     }
//   }
  
//   return 1;
// }


