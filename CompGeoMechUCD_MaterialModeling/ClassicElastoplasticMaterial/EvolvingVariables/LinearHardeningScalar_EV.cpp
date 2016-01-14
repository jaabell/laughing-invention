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

#include "LinearHardeningScalar_EV.h"
#include "Vector.h"

double LinearHardeningScalar_EV::derivative = 0.0;

LinearHardeningScalar_EV::LinearHardeningScalar_EV( double H_) : EvolvingVariable(0.0), H(H_)
{}

LinearHardeningScalar_EV::LinearHardeningScalar_EV( double H_, double k0) : EvolvingVariable(k0), H(H_)
{}

const double& LinearHardeningScalar_EV::getDerivative(const DTensor2 &depsilon,
        const DTensor2 &m,
        const DTensor2& stress) const
{
    using namespace ClassicElastoplasticityGlobals;
    // Clear the static variables
    derivative = 0;

    //Now compute the equivalent m
    double m_eq = sqrt(2 * m(i, j) * m(i, j) / 3);

    //Compute the derivative.
    derivative = H * m_eq;
    return derivative ;
}



int LinearHardeningScalar_EV::sendSelf(int commitTag, Channel &theChannel)
{
    //Shove all data into single vector for sending
    static Vector data(3);
    const double &a = this->getVariableConstReference();
    const double &a_committed = this->getVariableConstReference();

    data(0) = H;
    data(1) = a;
    data(2) = a_committed;

    if (theChannel.sendVector(0, commitTag, data) != 0)
    {
        cerr << "LinearHardeningScalar_EV::sendSelf() - Failed sending data" << endl;
        return -1;
    }

    return 0;
}

int LinearHardeningScalar_EV::receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
    static Vector data(3);
    if (theChannel.receiveVector(0, commitTag, data) != 0)
    {
        cerr << "LinearHardeningScalar_EV::receiveSelf() - Failed recieving data" << endl;
        return -1;
    }

    //Extract data from vector
    int pos = 0;
    double tmp_a;
    double tmp_a_committed;
    H = data(0);
    tmp_a = data(1);
    tmp_a_committed = data(2);

    this->setVar(tmp_a);
    this->setCommittedVar(tmp_a_committed);

    return 0;
}