///////////////////////////////////////////////////////////////////////////////
//
// COPYLEFT (C):     :-))
//  ``This  source code is Copyrighted in U.S., by the The Regents of the University
//  of California, for an indefinite period, and anybody caught using it without our
//  permission,  will  be  mighty  good friends of ourn, cause we don't give a darn.
//  Hack  it.  Compile it. Debug it. Run it. Yodel it. Enjoy it. We wrote it, that's
//  all we wanted to do.'' bj
// PROJECT:           Object Oriented Finite Element Program
// FILE:              AccelerationField.h
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


#ifndef AccelerationField_h
#define AccelerationField_h

#include <DomainComponent.h>
class Vector;
class Channel;

class AccelerationField : public DomainComponent
{
    public:
        AccelerationField(int tag, double bodyforce1, double bodyforce2, double bodyforce3);

        virtual ~AccelerationField();

        virtual const Vector& getAccelerationFieldVector(void);

        int sendSelf(int commitTag, Channel& theChannel);
        int receiveSelf(int commitTag, Channel& theChannel,  FEM_ObjectBroker& theBroker);
        void Print(ostream& s, int flag = 0);

    protected:

    private:

        static Vector bf;

};

#endif

