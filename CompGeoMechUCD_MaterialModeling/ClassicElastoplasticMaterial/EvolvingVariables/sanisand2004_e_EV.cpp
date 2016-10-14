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

#include "sanisand2004_e_EV.h"
#include "Vector.h"

double sanisand2004_e_EV::derivative = 0.0;


sanisand2004_e_EV::sanisand2004_e_EV(double e_in)
    :
    EvolvingVariable(e_in)
{}

const double& sanisand2004_e_EV::getDerivative(const DTensor2 &depsilon,
        const DTensor2 &m,
        const DTensor2& stress) const
{

    // This is not an actual internal variable, so doesnt evolve with the plasticity
    derivative = 0;
    return derivative ;
}

void sanisand2004_e_EV::evolve(double dlambda,
                               const DTensor2& depsilon,
                               const DTensor2& m,
                               const DTensor2& sigma)
{
    // e is not a proper internal variable, but we need to keep track of a committed version of this variable
    // and change it accordingly when using iterative solvers.
    // It would be messier to implement this outside an evolving variable.
    using namespace ClassicElastoplasticityGlobals;
    double volumetric_strain_increment = depsilon(i, i);
    const double &e = this->getVariableConstReference();
    double new_e = e + volumetric_strain_increment;
    this->setVar(new_e);
}

int sanisand2004_e_EV::sendSelf(int commitTag, Channel &theChannel)
{
    //Shove all data into single vector for sending
    static Vector data(2);
    const double &e = this->getVariableConstReference();
    const double &e_committed = this->getVariableConstReference();

    data(0) = e;
    data(1) = e_committed;

    if (theChannel.sendVector(0, commitTag, data) != 0)
    {
        cerr << "sanisand2004_e_EV::sendSelf() - Failed sending data" << endl;
        return -1;
    }

    return 0;
}

int sanisand2004_e_EV::receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
    static Vector data(2);
    if (theChannel.receiveVector(0, commitTag, data) != 0)
    {
        cerr << "sanisand2004_e_EV::receiveSelf() - Failed recieving data" << endl;
        return -1;
    }

    //Extract data from vector
    double tmp_e;
    double tmp_e_committed;
    tmp_e = data(0) ;
    tmp_e_committed = data(1) ;

    this->setVar(tmp_e);
    this->setCommittedVar(tmp_e_committed);

    return 0;
}