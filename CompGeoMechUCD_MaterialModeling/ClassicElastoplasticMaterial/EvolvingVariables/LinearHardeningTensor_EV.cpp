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


#include "LinearHardeningTensor_EV.h"
#include <Vector.h>

DTensor2 LinearHardeningTensor_EV::derivative(3, 3, 0.0);

LinearHardeningTensor_EV::LinearHardeningTensor_EV( double H_) : EvolvingVariable(DTensor2(3, 3, 0.0)), H(H_)
{}

LinearHardeningTensor_EV::LinearHardeningTensor_EV( double H_, DTensor2& alpha0) : EvolvingVariable(alpha0), H(H_)
{}

const DTensor2& LinearHardeningTensor_EV::getDerivative(const DTensor2 &depsilon,
        const DTensor2 &m,
        const DTensor2& stress) const
{
    using namespace ClassicElastoplasticityGlobals;
    //Zero de static variable
    derivative *= 0;

    //Compute the derivative (hardening function)
    double mkk = - (m(k, k) / 3);
    derivative(i, j) =  H * (m(i, j) + mkk * kronecker_delta(i, j));

    return derivative;
}


int LinearHardeningTensor_EV::sendSelf(int commitTag, Channel &theChannel)
{
    //Shove all data into single vector for sending
    static Vector data(9 + 9 + 1);
    const DTensor2 &a = this->getVariableConstReference();
    const DTensor2 &a_committed = this->getVariableConstReference();
    int pos = 0;

    data(pos++) = H;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
        {
            data(pos++) = a(i, j);
        }
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
        {
            data(pos++) = a_committed(i, j);
        }
    if (theChannel.sendVector(0, commitTag, data) != 0)
    {
        cerr << "LinearHardeningTensor_EV::sendSelf() - Failed sending data" << endl;
        return -1;
    }

    return 0;
}

int LinearHardeningTensor_EV::receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
    static Vector data(9 + 9 + 1);
    if (theChannel.receiveVector(0, commitTag, data) != 0)
    {
        cerr << "LinearHardeningTensor_EV::receiveSelf() - Failed recieving data" << endl;
        return -1;
    }

    //Extract data from vector
    int pos = 0;
    static DTensor2 tmp_a(3, 3, 0);
    static DTensor2 tmp_a_committed(3, 3, 0);
    H = data(pos++);
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
        {
            tmp_a(i, j) = data(pos++);
        }
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
        {
            tmp_a_committed(i, j) = data(pos++) ;
        }

    this->setVar(tmp_a);
    this->setCommittedVar(tmp_a_committed);

    return 0;
}