///////////////////////////////////////////////////////////////////////////////
//
// COPYLEFT (C):     :-))
//  ``This  source code is Copyrighted in U.S., by the The Regents of the University
//  of California, for an indefinite period, and anybody caught using it without our
//  permission,  will  be  mighty  good friends of ourn, cause we don't give a darn.
//  Hack  it.  Compile it. Debug it. Run it. Yodel it. Enjoy it. We wrote it, that's
//  all we wanted to do.'' bj
// PROJECT:           Object Oriented Finite Element Program
// FILE:              Rayleigh.cpp
// CLASS:             Rayleigh
// MEMBER FUNCTIONS:
//
// MEMBER VARIABLES
//
// PURPOSE:           Adding Rayleigh Damping to Domain
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
Rayleigh::recvSelf(int commitTag, Channel& theChannel,  FEM_ObjectBroker& theBroker)
{
    return 0;
}

void
Rayleigh::Print(ostream& s, int flag)
{

}


