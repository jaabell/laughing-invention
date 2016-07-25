///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):     :-))
// PROJECT:           Object Oriented Finite Element Program
// FILE:              UmfpackGenLinSolver.h
// CLASS:             UmfpackGenLinSolver
// MEMBER FUNCTIONS:
//
// MEMBER VARIABLES
//
// PURPOSE:           Finite Element Class
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:         DOS || UNIX || . . .
// PROGRAMMER:        Nima Tafazzoli and Boris Jeremic
// DATE:              April 2011
// UPDATE HISTORY:
//
///////////////////////////////////////////////////////////////////////////////
//
// Writing of this program was initiated by Frank McKenna in September 2000
// and then completed and linked to MOSS for UMFPACK5.5.1
// by Nima Tafazzoli and Boris Jeremic in April 2011
//
// Description: This file contains the class definition for
// UmfpackGenLinSolver. It solves the UmfpackGenLinSOEobject by calling
// UMFPACK5.5.1 routines.



#ifndef UmfpackGenLinSolver_h
#define UmfpackGenLinSolver_h

#include <LinearSOESolver.h>

class UmfpackGenLinSOE;

class UmfpackGenLinSolver : public LinearSOESolver
{
    public:
        UmfpackGenLinSolver();
        ~UmfpackGenLinSolver();

        int solve(void);
        int setSize(void);

        int setLinearSOE(UmfpackGenLinSOE& theSOE);

        int sendSelf(int commitTag, Channel& theChannel);
        int receiveSelf(int commitTag, Channel& theChannel,
                     FEM_ObjectBroker& theBroker);

    protected:

    private:
        double* Control;
        double* Info;
        void*   Symbolic;
        void*   Numeric;

        UmfpackGenLinSOE* theSOE;
};

#endif

