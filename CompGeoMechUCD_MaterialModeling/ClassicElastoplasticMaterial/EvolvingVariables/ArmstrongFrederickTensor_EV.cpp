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


#include "ArmstrongFrederickTensor_EV.h"
#include <Vector.h>

DTensor2 ArmstrongFrederickTensor_EV::derivative(3, 3, 0.0);

ArmstrongFrederickTensor_EV::ArmstrongFrederickTensor_EV( double ha_, double cr_) : EvolvingVariable(DTensor2(3, 3, 0.0)), ha(ha_), cr(cr_)
{}

ArmstrongFrederickTensor_EV::ArmstrongFrederickTensor_EV( double ha_, double cr_, DTensor2& alpha0) : EvolvingVariable(alpha0), ha(ha_), cr(cr_)
{}

const DTensor2& ArmstrongFrederickTensor_EV::getDerivative(const DTensor2 &depsilon,
        const DTensor2 &m,
        const DTensor2& stress) const
{
    using namespace ClassicElastoplasticityGlobals;
    //Zero de static variable
    derivative *= 0;

    //Compute the derivative (hardening function)
    const DTensor2 &alpha = this->getVariableConstReference();
    static DTensor2 mdev(3, 3, 0);
    mdev *= 0;
    mdev(i, j) = m(i, j) - m(k, k) / 3 * kronecker_delta(i, j);
    derivative(i, j) =  (2. / 3.) * ha * m(i, j) - cr * sqrt((2. / 3.) * m(k, l) * m(k, l)) * alpha(i, j);

    return derivative;
}

int ArmstrongFrederickTensor_EV::sendSelf(int commitTag, Channel &theChannel)
{
    //Shove all data into single vector for sending
    static Vector data(9 + 9 + 2);
    const DTensor2 &a = this->getVariableConstReference();
    const DTensor2 &a_committed = this->getVariableConstReference();
    int pos = 0;

    data(pos++) = ha;
    data(pos++) = cr;
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
        cerr << "ArmstrongFrederickTensor_EV::sendSelf() - Failed sending data" << endl;
        return -1;
    }

    return 0;
}

int ArmstrongFrederickTensor_EV::receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
    static Vector data(9 + 9 + 2);
    if (theChannel.receiveVector(0, commitTag, data) != 0)
    {
        cerr << "ArmstrongFrederickTensor_EV::receiveSelf() - Failed recieving data" << endl;
        return -1;
    }

    //Extract data from vector
    int pos = 0;
    static DTensor2 tmp_a(3, 3, 0);
    static DTensor2 tmp_a_committed(3, 3, 0);
    ha = data(pos++);
    cr = data(pos++);

    // TODO: This loop should get taken care of by the variables themselves.
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