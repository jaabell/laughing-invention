///////////////////////////////////////////////////////////////////////////////
//
// COPYLEFT (C):     :-))
//  ``This  source code is Copyrighted in U.S., by the The Regents of the University
//  of California, for an indefinite period, and anybody caught using it without our
//  permission,  will  be  mighty  good friends of ourn, cause we don't give a darn.
//  Hack  it.  Compile it. Debug it. Run it. Yodel it. Enjoy it. We wrote it, that's
//  all we wanted to do.'' bj
// PROJECT:           Object Oriented Finite Element Program
// FILE:              Damping.h
// CLASS:             Damping
// MEMBER FUNCTIONS:
//
// MEMBER VARIABLES
//
// PURPOSE:           Adding Damping to Domain
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:         DOS || UNIX || . . .
// DESIGNER:          Nima Tafazzoli, Boris Jeremic
// PROGRAMMER:        Nima Tafazzoli, Boris Jeremic
// DATE:              Oct. 2010
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////


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



