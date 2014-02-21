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
// Description: This file contains the class definition for ProfileSPDLinSOE
// ProfileSPDLinSOEGather is a subclass of LinearSOE. It uses the LAPACK Upper storage
// scheme to store the components of the A matrix.

// What: "@(#) ProfileSPDLinSOEGather.h, revA"

#ifndef ProfileSPDLinSOEGather_h
#define ProfileSPDLinSOEGather_h

#include <FEM_ObjectBroker.h>
#include <LinearSOE.h>
#include <Vector.h>
class ProfileSPDLinSolverGather;
class Channel;

class ProfileSPDLinSOEGather : public LinearSOE
{
    public:
        ProfileSPDLinSOEGather(ProfileSPDLinSolverGather& theSolver,
                               int numChannel, Channel** theChanels);

        ~ProfileSPDLinSOEGather();

        int solve(void);
        int getNumEqn(void) const;
        int setSize(Graph& theGraph);
        int addA(const Matrix&, const ID&, double fact = 1.0);
        int addB(const Vector&, const ID&, double fact = 1.0);
        int setB(const Vector&, double fact = 1.0);

        void zeroA(void);
        void zeroB(void);

        void setX(int loc, double value);
        void setX(const Vector& x);

        const Vector& getX(void);
        const Vector& getB(void);
        double normRHS(void);

        int setProfileSPDSolver(ProfileSPDLinSolverGather& newSolver);

        int sendSelf(int commitTag, Channel& theChannel);
        int recvSelf(int commitTag, Channel& theChannel, FEM_ObjectBroker& theBroker);

        friend class ProfileSPDLinSolverGather;

    protected:

    private:
        int size, profileSize;
        double* A, *B, *X;
        Vector* vectX;
        Vector* vectB;
        int* iDiagLoc;
        int Asize, Bsize;
        bool isAfactored, isAcondensed;
        int numInt;

        int numChannel;
        Channel** theChannels;
        double* work;
        FEM_ObjectBroker theBroker;
};


#endif



