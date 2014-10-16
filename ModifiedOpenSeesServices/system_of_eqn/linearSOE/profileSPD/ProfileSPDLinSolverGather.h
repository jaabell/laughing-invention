/* ****************************************************************** **
**    OpenSees - Open System for Earthquake Engineering Simulation    **
**          Pacific Earthquake Engineering Research Center            **
**                                                                    **
**                                                                    **
** (C) Copyright 1999, The Regents of the University of California    **
** All Rights Reserved.                                               **
**                                                                    **
** Commercial use of this program without express permission of the   **
** University of California, Berkeley, is strictly prohibited.  See   **
** file 'COPYRIGHT'  in main directory for information on usage and   **
** redistribution,  and for a DISCLAIMER OF ALL WARRANTIES.           **
**                                                                    **
** Developed by:                                                      **
**   Frank McKenna (fmckenna@ce.berkeley.edu)                         **
**   Gregory L. Fenves (fenves@ce.berkeley.edu)                       **
**   Filip C. Filippou (filippou@ce.berkeley.edu)                     **
**                                                                    **
** ****************************************************************** */

// $Revision$
// $Date$
// $Source$

// Written: fmk
// Revision: A
//
// What: "@(#) ProfileSPDLinDirectSolver.h, revA"

#ifndef ProfileSPDLinDirectSolver_h
#define ProfileSPDLinDirectSolver_h

#include <ProfileSPDLinSolver.h>
class ProfileSPDLinSOEGather;

class ProfileSPDLinSolverGather : public LinearSOESolver
{
    public:
        ProfileSPDLinSolverGather(double tol = 1.0e-12);
        virtual ~ProfileSPDLinSolverGather();

        virtual int solve(void);
        virtual int setSize(void);
        double getDeterminant(void);

        virtual int factor(int n);
        virtual int setLinearSOE(ProfileSPDLinSOEGather& theSOE);

        int sendSelf(int commitTag, Channel& theChannel);
        int receiveSelf(int commitTag, Channel& theChannel, FEM_ObjectBroker& theBroker);

    protected:
        ProfileSPDLinSOEGather* theSOE;
        double minDiagTol;
        int size;
        int* RowTop;
        double** topRowPtr, *invD;

    private:

};


#endif

