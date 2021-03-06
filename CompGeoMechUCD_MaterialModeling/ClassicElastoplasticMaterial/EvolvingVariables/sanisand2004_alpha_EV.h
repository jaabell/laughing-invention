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


#ifndef sanisand2004_alpha_EV_H
#define sanisand2004_alpha_EV_H

#include "../EvolvingVariable.h"
#include "../ClassicElastoplasticityGlobals.h" // Defines indices i,j,k,l,m,n,p,q,r,s and the kronecker_delta.


class sanisand2004_alpha_EV : public EvolvingVariable<DTensor2, sanisand2004_alpha_EV> //CRTP on sanisand2004_alpha_EV
{
public:

    sanisand2004_alpha_EV(double G0_, double h0_, double ch_, double patm_,
                          double Me_, double Mc_, double ec0_, double lambda_c_, double xi_, double nb_, double m_,
                          const double* e_ptr_,
                          const double* m_ptr_,
                          const DTensor2* alpha_in_ptr_);
    sanisand2004_alpha_EV(double G0_, double h0_, double ch_, double patm_,
                          double Me_, double Mc_, double ec0_, double lambda_c_, double xi_, double nb_, double m_,
                          const double* e_ptr_,
                          const double* m_ptr_,
                          const DTensor2* alpha_in_ptr_,
                          DTensor2& alpha0);

    const DTensor2& getDerivative(const DTensor2 &depsilon,
                                  const DTensor2 &m,
                                  const DTensor2& stress) const;

    int sendSelf(int commitTag, Channel &theChannel);
    int receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker);

private:
    //Hardening parameters
    double G0, h0, ch, patm;

    //Critical state parameters
    double Me, Mc, ec0, lambda_c, xi, nb;

    //References to other internal variables in the model (Pointers have to be used here so that default assignment operator works)
    const double *e_ptr;
    const double * m_ptr;
    const DTensor2* alpha_in_ptr;
};



#endif