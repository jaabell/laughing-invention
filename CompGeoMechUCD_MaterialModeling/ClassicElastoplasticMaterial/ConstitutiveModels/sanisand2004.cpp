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

#include "sanisand2004.h"


// sanisand2004_alpha_EV( G0_,  h0_,  ch_,  patm_,
//                        Me_,  Mc_,  ec0_,  lambda_c_,  xi_,  nb_,  m_,
//                       e_,
//                       alpha_in)
// sanisand2004_alpha_EV( G0_,  h0_,  ch_,  patm_,
//                        Me_,  Mc_,  ec0_,  lambda_c_,  xi_,  nb_,  m_,
//                       e_,
//                       alpha_in,
//                       alpha0)


// sanisand2004_z_EV(double A0_, double cz_, double zmax_, double patm_,
//                   double Me_, double Mc_, double ec0_, double lambda_c_, double xi_, double nd_,
//                   const double& m_,
//                   const double& e_,
//                   const DTensor2& alpha_);
// sanisand2004_z_EV(double A0_, double cz_, double zmax_, double patm_,
//                   double Me_, double Mc_, double ec0_, double lambda_c_, double xi_, double nd_,
//                   const double& m_,
//                   const double& e_,
//                   const DTensor2& alpha_,
//                   DTensor2& alpha0);

// sanisand2004_z_EV( A0_,  cz_,  zmax_,  patm_,
//                    Me_,  Mc_,  ec0_,  lambda_c_,  xi_,  nd_,
//                    m_,
//                    e_,
//                    alpha_);
// sanisand2004_z_EV( A0_,  cz_,  zmax_,  patm_,
//                    Me_,  Mc_,  ec0_,  lambda_c_,  xi_,  nd_,
//                    m_,
//                    e_,
//                    alpha_,
//                    DTensor2& alpha0);

// sanisand2004_EL(double G0_in, double patm_in, double nu_in, const double& e_in);

//First constructor, creates a material at its "ground state" from its parameters.
sanisand2004::sanisand2004(int tag_in, double rho_, double initial_confinement_p0,
                           double G0_in, double patm_in, double nu_in,
                           double h0_in, double ch_in,
                           double Me_in, double Mc_in, double ec0_in, double lambda_c_in,
                           double xi_in,
                           double nb_in,
                           double A0_in, double cz_in, double zmax_in, double nd_in,
                           double m_in, double e_in) :
    sanisand2004Base::ClassicElastoplasticMaterial(tag_in, rho_, initial_confinement_p0,
            sanisand2004_YFType(alpha, m),       // Point YF to internal variables
            sanisand2004_EL(G0_in, patm_in, nu_in, e), // Create Elasticity
            sanisand2004_PFType(alpha, z, m),       // Point PF to the internal variables
            sanisand2004VarsType(alpha, z,  e, m)),     // Declare the list of internal variables
    alpha( G0_in,  h0_in,  ch_in,  patm_in,
           Me_in,  Mc_in,  ec0_in,  lambda_c_in,  xi_in,  nb_in,
           m.getVariableConstReference(),
           &e.getVariableConstReference(),
           &alpha.getVariableConstReference()),
    z(A0_in,  cz_in,  zmax_in,  patm_in,
      Me_in,  Mc_in,  ec0_in,  lambda_c_in,  xi_in,  nd_in,
      &m.getVariableConstReference(),
      &e.getVariableConstReference(),
      &alpha.getVariableConstReference()),
    m(0.0, m_in),
    e(e_in)
{

}

// Second constructor is not called by the user, instead it is called when creating a copy of the
// material. This must provide an initialization for the state variables and link the components
// to these variables appropriately.
sanisand2004::sanisand2004(int tag_in, double rho_, double initial_confinement_p0, sanisand2004_YFType &yf,
                           sanisand2004_EL &el,
                           sanisand2004_PFType &pf,
                           sanisand2004VarsType &vars) :
    sanisand2004Base::ClassicElastoplasticMaterial(tag_in, this->getRho(),
            initial_confinement_p0,     //Sets p0
            sanisand2004_YFType(alpha, m),    // Point YF to new internal variables
            sanisand2004_EL(0, 0, 0, e), // Create Elasticity -- use copy constructor here
            sanisand2004_PFType(alpha, z, m),    // Point PF to the internal variables
            sanisand2004VarsType(alpha, z, e, m)),   // Declare the list of internal variables
    alpha( 0.0,  0.0,  0.0,  0.0,
           0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
           m.getVariableConstReference(),
           &e.getVariableConstReference(), //Need to dereference these pointers
           &alpha.getVariableConstReference()), //Need to dereference these pointers
    z(0.0,  0.0,  0.0,  0.0,
      0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      &m.getVariableConstReference(),
      &e.getVariableConstReference(),
      &alpha.getVariableConstReference()),
    m(0.0, 0.0),
    e(0.0)
{

}

sanisand2004::sanisand2004() :
    sanisand2004Base::ClassicElastoplasticMaterial(0, 0, 0,
            sanisand2004_YFType(alpha, m),    // Point YF to new internal variables
            sanisand2004_EL(0, 0, 0, e), // Create Elasticity -- use copy constructor here
            sanisand2004_PFType(alpha, z, m),    // Point PF to the internal variables
            sanisand2004VarsType(alpha, z, e, m)),   // Declare the list of internal variables
    alpha( 0.0,  0.0,  0.0,  0.0,
           0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
           m.getVariableConstReference(),
           &e.getVariableConstReference(),
           &alpha.getVariableConstReference()),
    z(0.0,  0.0,  0.0,  0.0,
      0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      &m.getVariableConstReference(),
      &e.getVariableConstReference(),
      &alpha.getVariableConstReference()),
    m(0.0, 0.0),
    e(0.0)
{

}


void
sanisand2004::Print(ostream& s, int flag)
{
    s << "sanisand2004" << endln;
    s << "\tTag: " << this->getTag() << endln;
    s << " Please Implement Me !!! " << endl;
}