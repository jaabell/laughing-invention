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





#include <Damping.h>


Damping::Damping(int tag, int clasTag)
    : DomainComponent(tag, clasTag),
      a0(0.0), a1(0.0), a2(0.0), a3(0.0)
{
    // does nothing
}


Damping::~Damping()
{
    // does nothing
}



double
Damping::return_a0()
{
    return this->return_a0();
}


double
Damping::return_a1()
{
    return this->return_a1();
}


double
Damping::return_a2()
{
    return this->return_a2();
}


double
Damping::return_a3()
{
    return this->return_a3();
}


string
Damping::return_dampingtype()
{
    return this->return_dampingtype();
}


string
Damping::return_stiffnesstype()
{
    return this->return_stiffnesstype();
}


int
Damping::return_tag()
{
    return this->return_tag();
}



