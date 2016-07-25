///////////////////////////////////////////////////////////////////////////////
//
// COPYLEFT (C):     :-))
//  ``This  source code is Copyrighted in U.S., by the The Regents of the University
//  of California, for an indefinite period, and anybody caught using it without our
//  permission,  will  be  mighty  good friends of ourn, cause we don't give a darn.
//  Hack  it.  Compile it. Debug it. Run it. Yodel it. Enjoy it. We wrote it, that's
//  all we wanted to do.'' bj
// PROJECT:           Object Oriented Finite Element Program
// FILE:              AccelerationField.cpp
// CLASS:             AccelerationField
// MEMBER FUNCTIONS:
//
// MEMBER VARIABLES
//
// PURPOSE:           Adding Body forces to Domain
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


#ifndef AccelerationField_CPP
#define AccelerationField_CPP


#include <AccelerationField.h>
#include <Vector.h>
#include <Channel.h>
#include <FEM_ObjectBroker.h>

Vector AccelerationField::bf(3);

AccelerationField::AccelerationField(int tag, double b1, double b2, double b3)
    : DomainComponent(tag, Load_TAG_AccelerationField)
{
    bf(0) = b1;
    bf(1) = b2;
    bf(2) = b3;
}


AccelerationField::~AccelerationField()
{
}

const Vector&
AccelerationField::getAccelerationFieldVector()
{
    return bf;
}


int
AccelerationField::sendSelf(int commitTag, Channel& theChannel)
{
    return 0;
}

int
AccelerationField::receiveSelf(int commitTag, Channel& theChannel,  FEM_ObjectBroker& theBroker)
{
    return 0;
}

void
AccelerationField::Print(ostream& s, int flag)
{

}



#endif

