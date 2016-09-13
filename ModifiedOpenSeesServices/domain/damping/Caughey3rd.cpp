///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Nima Tafazzoli, Boris Jeremic
// DATE:               Nov 2010
// UPDATE HISTORY:     Full update history in git repository.
// QUALITY ASSURANCE:  Developers have worked really hard to develop
//                     an extensive verification of developed implementation
//                     and with that can claim quality and fitness for intended
//                     purpose (modeling and simulation of Real ESSI Problems)
//                     within confines of verification effort
//
// LEGACY/DEFUNCT COPYLEFT (C):
//                     Woody's viral GPL-like license (adapted by BJ):
//                     ``This    source  code is Copyrighted in
//                     worldwide for  an  indefinite  period,  and anybody
//                     caught  using it without our permission, will be
//                     mighty good friends of ourn, cause we don't give
//                     a  darn.  Hack it. Compile it. Debug it. Run it.
//                     Yodel  it.  Enjoy it. We wrote it, that's all we
//                     wanted to do.''
//
/////////////////////////////////////////////////////////////////////////////





#include <Caughey3rd.h>


Caughey3rd::Caughey3rd(int tag, double A0, double A1, double A2, string Stiffness_Type)
    : Damping(tag, RANDOM_VARIABLE_caughey3rd), damptag(tag),
      a0(A0), a1(A1), a2(A2), stiffness_type(Stiffness_Type)
{

}


double
Caughey3rd::return_a0()
{
    return a0;
}


double
Caughey3rd::return_a1()
{
    return a1;
}


double
Caughey3rd::return_a2()
{
    return a2;
}


double
Caughey3rd::return_a3()
{
    return 0;
}


int
Caughey3rd::return_tag()
{
    return damptag;
}


string
Caughey3rd::return_stiffnesstype()
{
    return stiffness_type;
}


int
Caughey3rd::sendSelf(int commitTag, Channel& theChannel)
{
    return 0;
}

int
Caughey3rd::receiveSelf(int commitTag, Channel& theChannel,  FEM_ObjectBroker& theBroker)
{
    return 0;
}

void
Caughey3rd::Print(ostream& s, int flag)
{

}


