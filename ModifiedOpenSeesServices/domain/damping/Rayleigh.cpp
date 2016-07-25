///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):      Version of a Creative Commons License,
//                     for details contact Boris Jeremic, jeremic@ucdavis.edu
// PROJECT:            Real ESSI Simulator
// PROGRAMMER:         Nima Tafazzoli, Boris Jeremic
// DATE:               Oct 2010
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



#include <Rayleigh.h>


Rayleigh::Rayleigh(int tag, double A0, double A1, string Stiffness_Type)
    : Damping(tag, RANDOM_VARIABLE_rayleigh), damptag(tag),
      a0(A0), a1(A1), stiffness_type(Stiffness_Type)
{

}


double
Rayleigh::return_a0()
{
    return a0;
}


double
Rayleigh::return_a1()
{
    return a1;
}


double
Rayleigh::return_a2()
{
    return 0;
}


double
Rayleigh::return_a3()
{
    return 0;
}


int
Rayleigh::return_tag()
{
    return damptag;
}


string
Rayleigh::return_stiffnesstype()
{
    return stiffness_type;
}


int
Rayleigh::sendSelf(int commitTag, Channel& theChannel)
{
    return 0;
}

int
Rayleigh::receiveSelf(int commitTag, Channel& theChannel,  FEM_ObjectBroker& theBroker)
{
    return 0;
}

void
Rayleigh::Print(ostream& s, int flag)
{

}


